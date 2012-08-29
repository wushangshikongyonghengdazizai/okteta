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
#ifndef PRIMITIVEARRAYDATA_H
#define PRIMITIVEARRAYDATA_H

#include "abstractarraydata.h"
#include "../datainformation.h"
#include "../dummydatainformation.h"
#include "../topleveldatainformation.h"
#include "../primitive/primitivetemplateinfo.h"

template<PrimitiveDataTypeEnum type>
class PrimitiveArrayData : public AbstractArrayData
{
    Q_DISABLE_COPY(PrimitiveArrayData)
public:
    typedef typename PrimitiveInfo<type>::valueType T;
    typedef typename PrimitiveInfo<type>::Class DisplayClass;

    explicit PrimitiveArrayData(unsigned int initialLength, DataInformation* parent);
    virtual ~PrimitiveArrayData();

    virtual qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
            BitCount64 bitsRemaining);
    virtual bool setChildData(uint row, QVariant value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining);

    virtual DataInformation* childAt(unsigned int idx);
    virtual QVariant dataAt(uint index, int column, int role);
    AllPrimitiveTypes valueAt(int index) const;
    virtual int indexOf(const DataInformation* data) const;
    virtual unsigned int length() const;
    virtual void setLength(uint newLength);
    virtual BitCount32 offset(uint row) const;
    virtual BitCount32 size() const;
    virtual PrimitiveDataType primitiveType() const;
    virtual void setParent(DataInformation* parent);
    virtual BitCount32 sizeAt(uint index);
    virtual Qt::ItemFlags childFlags(int row, int column, bool fileLoaded);

    virtual QScriptValue toScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) const;
    virtual QString typeName() const;

    virtual bool isComplex() const;


    virtual QWidget* createChildEditWidget(uint index, QWidget* parent) const;
    virtual QVariant dataFromChildWidget(uint index, const QWidget* w) const;
    virtual void setChildWidgetData(uint index, QWidget* w) const;

    static void writeOneItem(T value, Okteta::Address addr, Okteta::AbstractByteArrayModel* out, bool littleEndian);
protected:
    /** reads @p numItems items from the input, sizes must have been checked before calling this method!! */
    void readDataNativeOrder(uint numItems, Okteta::AbstractByteArrayModel* input, Okteta::Address addr);
    /** reads @p numItems items from the input, sizes must have been checked before calling this method!! */
    void readDataNonNativeOrder(uint numItems, Okteta::AbstractByteArrayModel* input, Okteta::Address addr);

    QVector<T> mData;
    uint mNumReadValues; //the number of values read before EOF
    QScopedPointer<DummyDataInformation> mDummy;
};

//constructors
template<PrimitiveDataTypeEnum type>
inline PrimitiveArrayData<type>::PrimitiveArrayData(unsigned int initialLength, DataInformation* parent)
        : AbstractArrayData(parent), mNumReadValues(0), mDummy(new DummyDataInformation(parent))
{
    mData.reserve(initialLength);
    mData.resize(initialLength);
}

template<PrimitiveDataTypeEnum type>
inline PrimitiveArrayData<type>::~PrimitiveArrayData()
{
}

template<PrimitiveDataTypeEnum type>
inline void PrimitiveArrayData<type>::setParent(DataInformation* parent)
{
    mParent = parent;
    mDummy->setParent(parent);
}

template<PrimitiveDataTypeEnum type>
inline BitCount32 PrimitiveArrayData<type>::offset(uint index) const
{
    Q_ASSERT(index < length());
    return index * sizeof(T) * 8;
}

template<PrimitiveDataTypeEnum type>
inline DataInformation* PrimitiveArrayData<type>::childAt(unsigned int idx)
{
    Q_ASSERT(idx < length());
    Q_UNUSED(idx);
    return mDummy.data();
}

template<PrimitiveDataTypeEnum type>
inline unsigned int PrimitiveArrayData<type>::length() const
{
    return mData.size();
}

template<PrimitiveDataTypeEnum type>
inline BitCount32 PrimitiveArrayData<type>::size() const
{
    return mData.size() * sizeof(T) * 8;
}

template<PrimitiveDataTypeEnum type>
void PrimitiveArrayData<type>::setLength(uint newLength)
{
    mData.resize(newLength);
    mData.squeeze();
}

template<PrimitiveDataTypeEnum type>
inline PrimitiveDataType PrimitiveArrayData<type>::primitiveType() const
{
    return type;
}

template<PrimitiveDataTypeEnum type>
inline BitCount32 PrimitiveArrayData<type>::sizeAt(uint index)
{
    Q_ASSERT(index < length());
    Q_UNUSED(index)
    return sizeof(T) * 8;
}

template<PrimitiveDataTypeEnum type>
inline Qt::ItemFlags PrimitiveArrayData<type>::childFlags(int row, int column, bool fileLoaded)
{
    Q_ASSERT(row >= 0 && uint(row) < length());
    Q_UNUSED(row)
    if (column == 2 && fileLoaded)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

template<PrimitiveDataTypeEnum type>
bool PrimitiveArrayData<type>::isComplex() const
{
    return false;
}

template<PrimitiveDataTypeEnum type>
AllPrimitiveTypes PrimitiveArrayData<type>::valueAt(int index) const
{
    Q_ASSERT(index >= 0 && index < mData.size());
    return AllPrimitiveTypes(mData.at(index));
}





#endif // PRIMITIVEARRAYDATA_H
