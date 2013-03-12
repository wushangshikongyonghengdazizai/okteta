/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "defaultscriptclass.h"

#include "../../datatypes/datainformation.h"
#include "../../datatypes/topleveldatainformation.h"
#include "../../datatypes/uniondatainformation.h"
#include "../../datatypes/structuredatainformation.h"
#include "../../datatypes/primitive/pointerdatainformation.h"
#include "../../parsers/parserutils.h"
#include "../../parsers/scriptvalueconverter.h"
#include "../scriptlogger.h"
#include "../scripthandlerinfo.h"

#include <KDebug>

DefaultScriptClass::DefaultScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
    : QScriptClass(engine), mHandlerInfo(handlerInfo)
{
    s_valid = engine->toStringHandle(ParserStrings::PROPERTY_VALID);
    s_wasAbleToRead = engine->toStringHandle(ParserStrings::PROPERTY_ABLE_TO_READ);
    s_validationError = engine->toStringHandle(ParserStrings::PROPERTY_VALIDATION_ERROR);
    s_parent = engine->toStringHandle(ParserStrings::PROPERTY_PARENT);
    s_byteOrder = engine->toStringHandle(ParserStrings::PROPERTY_BYTEORDER);
    s_name = engine->toStringHandle(ParserStrings::PROPERTY_NAME);
    s_datatype = engine->toStringHandle(ParserStrings::PROPERTY_DATATYPE);
    s_updateFunc = engine->toStringHandle(ParserStrings::PROPERTY_UPDATE_FUNC);
    s_validationFunc = engine->toStringHandle(ParserStrings::PROPERTY_VALIDATION_FUNC);
    s_customTypeName = engine->toStringHandle(ParserStrings::PROPERTY_CUSTOM_TYPE_NAME);
    qScriptRegisterMetaType<DataInfPtr>(engine, DefaultScriptClass::toScriptValue, DefaultScriptClass::fromScriptValue);

    //TODO remove, every subclass should have proto
    mDefaultPrototype = engine->newObject();
    mDefaultPrototype.setProperty(QLatin1String("toString"), engine->newFunction(Default_proto_toString));
    //add all our properties
    mIterableProperties.append(qMakePair(s_parent, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    mIterableProperties.append(qMakePair(s_name, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    mIterableProperties.append(qMakePair(s_wasAbleToRead, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    mIterableProperties.append(qMakePair(s_byteOrder, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    mIterableProperties.append(qMakePair(s_valid, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    mIterableProperties.append(qMakePair(s_validationError, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    mIterableProperties.append(qMakePair(s_validationFunc, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    mIterableProperties.append(qMakePair(s_updateFunc, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    mIterableProperties.append(qMakePair(s_datatype, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    mIterableProperties.append(qMakePair(s_customTypeName, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
}

DefaultScriptClass::~DefaultScriptClass()
{
}

QScriptValue DefaultScriptClass::toScriptValue(QScriptEngine* eng, const DataInfPtr& data)
{
    QScriptValue obj = eng->newObject();
    obj.setData(eng->newVariant(QVariant::fromValue(data)));
    return obj;
}

void DefaultScriptClass::fromScriptValue(const QScriptValue& obj, DataInfPtr& data)
{
    data = qvariant_cast<DataInfPtr>(obj.data().toVariant());
}

QScriptClass::QueryFlags DefaultScriptClass::queryProperty(const QScriptValue& object,
            const QScriptString& name, QScriptClass::QueryFlags flags, uint* id)
{
    const ScriptHandlerInfo::Mode mode = mHandlerInfo->mode();
    Q_ASSERT(mode != ScriptHandlerInfo::None);
    DataInformation* data = qscriptvalue_cast<DataInformation*>(object.data());
    if (!data)
    {
        mHandlerInfo->logger()->error() << "could not cast data from" << object.toString();
        return 0;
    }
    if (name == s_valid || name == s_validationError)
    {
        return mode == ScriptHandlerInfo::Validating ? flags : flags & ~HandlesWriteAccess;
    }
    if (mode != ScriptHandlerInfo::Updating)
    {
        //the only properties that are possibly writable when not updating are valid and validationError
        //but we checked them before so we remove handlesWriteAccess from the flags
        flags &= ~HandlesWriteAccess;
    }

    if (name == s_byteOrder || name == s_name || name == s_updateFunc
            || name == s_validationFunc || name == s_datatype || name == s_customTypeName)
    {
        return flags;
    }
    else if (name == s_wasAbleToRead || name == s_parent)
    {
        return flags &= ~HandlesWriteAccess;
    }
    else if (queryAdditionalProperty(data, name, &flags, id))
    {
        return flags;
    }
    else
    {
        data->logError() << "could not find property with name" << name.toString();
        return 0;
    }
}

QScriptValue DefaultScriptClass::property(const QScriptValue& object, const QScriptString& name, uint id)
{
    Q_ASSERT(mHandlerInfo->mode() != ScriptHandlerInfo::None);
    DataInformation* data = qscriptvalue_cast<DataInformation*>(object.data());
    if (!data)
    {
        mHandlerInfo->logger()->error() << "could not cast data from" << object.toString();
        return QScriptValue();
    }
    if (name == s_valid)
    {
        return data->validationSuccessful();
    }
    else if (name == s_wasAbleToRead)
    {
        return data->wasAbleToRead();
    }
    else if (name == s_parent)
    {
        Q_CHECK_PTR(data->parent());
        //parent() cannot be null
        if (data->parent()->isTopLevel())
            return engine()->nullValue();
        return data->parent()->asDataInformation()->toScriptValue(engine(), mHandlerInfo);
    }
    else if (name == s_datatype)
    {
        return data->typeName();
    }
    else if (name == s_updateFunc)
    {
        return data->updateFunc();
    }
    else if (name == s_validationFunc)
    {
        return data->validationFunc();
    }
    else if (name == s_validationError)
    {
        return data->validationError();
    }
    else if (name == s_byteOrder)
    {
        return ParserUtils::byteOrderToString(data->byteOrder());
    }
    else if (name == s_name)
    {
        return data->name();
    }
    else if (name == s_customTypeName)
    {
        return data->typeName();
    }
    QScriptValue other = additionalProperty(data, name, id);
    if (other.isValid())
        return other;
    else
    {
        data->logError() << "could not find property with name" << name.toString();
        return engine()->undefinedValue();
    }
}

void DefaultScriptClass::setDataType(const QScriptValue& value, DataInformation* data)
{
    DataInformation* thisObj = qscriptvalue_cast<DataInformation*>(engine()->currentContext()->thisObject().data());
    Q_CHECK_PTR(thisObj);
    const bool isThisObj = thisObj == data;
    if (data->hasBeenUpdated())
    {
        //this element has already been updated (and probably read, replacing it could cause crazy errors
        data->logError() << "Attempting to replace an already updated object. This could cause errors."
                "Current this object: " << (thisObj ? thisObj->fullObjectPath() : QString());
        return;
    }
    //change the type of the underlying object
    DataInformation* newType = ScriptValueConverter::convert(value, data->name(), data->logger(), data);
    if (!newType)
    {
        data->logError() << "Failed to set new type, could not convert value!";
        return;
    }

    DataInformationBase* parent = data->parent();
    Q_CHECK_PTR(parent);
    TopLevelDataInformation* top = data->topLevelDataInformation();
    Q_CHECK_PTR(top);
    //only if parent is toplevel, struct or union, can we replace
    bool replaced = false;
    if (parent->isTopLevel())
    {
        parent->asTopLevel()->setActualDataInformation(newType);
        replaced = true;
    }
    else if (parent->isStruct())
    {
        StructureDataInformation* stru = parent->asStruct();
        int index = stru->indexOf(data);
        Q_ASSERT(index != -1);
        Q_ASSERT(uint(index) < stru->childCount());
        replaced = stru->replaceChildAt(index, newType);
        if (!replaced)
            stru->logError() << "failed to replace child at index" << index;
    }
    else if (parent->isUnion())
    {
        UnionDataInformation* un = parent->asUnion();
        int index = un->indexOf(data);
        Q_ASSERT(index != -1);
        Q_ASSERT(uint(index) < un->childCount());
        replaced = un->replaceChildAt(index, newType);
        if (!replaced)
            un->logError() << "failed to replace child at index" << index;
    }
    else if (parent->isPointer())
    {
        parent->asPointer()->setPointerTarget(newType);
        replaced = true;
    }
    else
    {
        data->logError() << "Failed to set data type since element is not toplevel and parent"
                " is neither struct nor union nor pointer.";
    }
    if (replaced)
    {
        top->setChildDataChanged();
        newType->mHasBeenUpdated = true;
        //if the current object was "this" in javascript we have to replace it
        if (isThisObj)
            engine()->currentContext()->setThisObject(newType->toScriptValue(engine(), mHandlerInfo));
    }
    else
    {
        delete newType; //could not set new type
    }
}

void DefaultScriptClass::setProperty(QScriptValue& object, const QScriptString& name, uint id, const QScriptValue& value)
{
    const ScriptHandlerInfo::Mode mode = mHandlerInfo->mode();
    Q_ASSERT(mode != ScriptHandlerInfo::None);
    DataInformation* data = qscriptvalue_cast<DataInformation*>(object.data());
    if (!data)
    {
        mHandlerInfo->logger()->error() << "could not cast data from" << object.toString();
        return;
    }
    if (mode == ScriptHandlerInfo::Validating)
    {
        //only way write access is allowed is when validating: valid and validationError
        if (data->hasBeenValidated())
            data->logError() << "Cannot modify this object, it has already been validated!";
        else if (name == s_valid)
            data->mValidationSuccessful = value.toBool();
        else if (name == s_validationError)
            data->setValidationError(value.toString());
        else
            data->logError() << "Cannot write to property" << name.toString() << "while validating!";
        return;
    }

    if (mode != ScriptHandlerInfo::Updating)
    {
        data->logError() << "Writing to property" << name.toString() << "is only allowed when updating.";
        return;
    }
    Q_ASSERT(mode == ScriptHandlerInfo::Updating);

    if (name == s_byteOrder)
    {
        data->setByteOrder(ParserUtils::byteOrderFromString(value.toString(),
                LoggerWithContext(data->logger(), data->fullObjectPath())));
    }
    else if (name == s_datatype)
    {
        //change the type of the underlying object
        setDataType(value, data);
    }
    else if (name == s_updateFunc)
    {
        data->setUpdateFunc(value);
    }
    else if (name == s_validationFunc)
    {
        data->setValidationFunc(value);
    }
    else if (name == s_name)
    {
        data->setName(value.toString());
    }
    else if (name == s_customTypeName)
    {
        if (!value.isValid() || value.isNull() || value.isUndefined())
            data->setCustomTypeName(QString()); //unset
        else
            data->setCustomTypeName(value.toString());
    }
    else if (name == s_wasAbleToRead || name == s_valid || name == s_validationError)
    {
        data->logError() << "Writing to property " << s_valid.toString() << "is not allowed when updating.";
        return; //can't write
    }
    else
    {
        bool setAdditional = setAdditionalProperty(data, name, id, value);
        if (setAdditional)
            return;
        else
        {
            data->logError() << "could not set property with name" << name.toString();
        }
    }
}

QScriptValue::PropertyFlags DefaultScriptClass::propertyFlags(const QScriptValue& object, const QScriptString& name, uint id)
{
    QScriptValue::PropertyFlags result;
    const ScriptHandlerInfo::Mode mode = mHandlerInfo->mode();
    Q_ASSERT(mode != ScriptHandlerInfo::None);
    DataInformation* data = qscriptvalue_cast<DataInformation*>(object.data());
    if (!data)
    {
        mHandlerInfo->logger()->error() << "could not cast data from" << object.toString();
        return 0;
    }
    if (name == s_valid || name == s_validationError)
    {
        if (mode != ScriptHandlerInfo::Validating)
            result |= QScriptValue::ReadOnly;
    }
    else if (mode != ScriptHandlerInfo::Updating)
    {
        result |= QScriptValue::ReadOnly;
    }

    for (int i = 0, size = mIterableProperties.size(); i < size; ++i) {
        if (mIterableProperties.at(i).first == name)
            return result | mIterableProperties.at(i).second;
    }
    if (additionalPropertyFlags(data, name, id, &result))
        return result; //is a child element
    else
    {
        data->logError() << "could not find flags for property with name" << name.toString();
        return 0;
    }
}

QScriptValue DefaultScriptClass::prototype() const
{
    return mDefaultPrototype;
}

QScriptValue DefaultScriptClass::Default_proto_toString(QScriptContext* ctx, QScriptEngine* eng)
{
    DataInformation* data = qscriptvalue_cast<DataInformation*>(ctx->thisObject().data());
    if (!data)
    {
        kWarning() << "could not cast data";
        return eng->undefinedValue();
    }
    return QString(data->typeName() + QLatin1Char(' ') + data->name());
}


QScriptClassPropertyIterator* DefaultScriptClass::newIterator(const QScriptValue& object)
{
    return new DefaultscriptClassIterator(object, mIterableProperties, engine());
}

DefaultscriptClassIterator::DefaultscriptClassIterator(const QScriptValue& object, const DefaultScriptClass::PropertyInfoList& list,
                                                       QScriptEngine* engine)
    : QScriptClassPropertyIterator(object), mCurrent(-1), mList(list), mEngine(engine)
{
    DataInformation* data = qscriptvalue_cast<DataInformation*>(object.data());
    Q_CHECK_PTR(data);
    mData = data;
}

DefaultscriptClassIterator::~DefaultscriptClassIterator()
{
}

bool DefaultscriptClassIterator::hasNext() const
{
    return mCurrent < mList.size() - 1;
}

bool DefaultscriptClassIterator::hasPrevious() const
{
    return mCurrent > 0;
}

QScriptString DefaultscriptClassIterator::name() const
{
    Q_ASSERT(mCurrent >= 0 && (uint)mCurrent < mList.size() + mData->childCount());
    if (mCurrent < 0 || (uint)mCurrent >= mList.size() + mData->childCount())
        return QScriptString();
    if (mCurrent < mList.size())
        return mList.at(mCurrent).first;
    int index = mCurrent - mList.size();
    Q_ASSERT(index >= 0);
    DataInformation* child = mData->childAt(index);
    return mEngine->toStringHandle(child->name());
}

QScriptValue::PropertyFlags DefaultscriptClassIterator::flags() const
{
    Q_ASSERT(mCurrent >= 0 && (uint)mCurrent < mList.size() + mData->childCount());
    if (mCurrent < 0 || (uint)mCurrent >= mList.size() + mData->childCount())
        return 0;
    if (mCurrent < mList.size())
        return mList.at(mCurrent).second;
    return QScriptValue::ReadOnly;
}

uint DefaultscriptClassIterator::id() const
{
    Q_ASSERT(mCurrent >= 0 && (uint)mCurrent < mList.size() + mData->childCount());
    if (mCurrent < 0 || (uint)mCurrent >= mList.size() + mData->childCount())
        return 0;
    //only children have an id assigned
    if (mCurrent < mList.size())
        return 0;
    return mCurrent - mList.size() + 1;
}

void DefaultscriptClassIterator::next()
{
    Q_ASSERT(mCurrent == -1 || (uint)mCurrent < mList.size() + mData->childCount());
    mCurrent++;
}

void DefaultscriptClassIterator::previous()
{
    Q_ASSERT(mCurrent >= 0);
    mCurrent--;
}

void DefaultscriptClassIterator::toBack()
{
    mCurrent = mList.size() + mData->childCount();
}

void DefaultscriptClassIterator::toFront()
{
    mCurrent = -1;
}



