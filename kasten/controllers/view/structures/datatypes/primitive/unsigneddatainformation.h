/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011  Alex Richardson <alex.richardson@gmx.de>
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


#ifndef UNSIGNEDDATAINFORMATION_H
#define UNSIGNEDDATAINFORMATION_H

#include "primitivedatainformation.h"
#include "../poddecoder/typeeditors/uintspinbox.h"

template<typename T>
class UnsignedDataInformation : public PrimitiveDataInformation
{
public:
    explicit UnsignedDataInformation(QString name, DataInformation* parent = 0);
    virtual ~UnsignedDataInformation() {}

    virtual int size() const;
    static int displayBase();

    virtual AllPrimitiveTypes value() const;
    virtual void setValue(AllPrimitiveTypes newVal);

    virtual QWidget* createEditWidget(QWidget* parent) const;
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual void setWidgetData(QWidget* w) const;
    virtual AllPrimitiveTypes qVariantToAllPrimitiveTypes(const QVariant& value) const;

    virtual QScriptValue valueAsQScriptValue() const = 0;
    virtual QString valueString() const = 0;
protected:
    explicit UnsignedDataInformation(const UnsignedDataInformation& d);
protected:
    T mValue;
};

template<typename T>
QWidget* UnsignedDataInformation<T>::createEditWidget(QWidget* parent) const
{
    UIntSpinBox* ret = new UIntSpinBox(parent, displayBase());
    ret->setMaximum(std::numeric_limits<T>::max());
    return ret;
}

template<typename T>
QVariant UnsignedDataInformation<T>::dataFromWidget(const QWidget* w) const
{
    const UIntSpinBox* spin = dynamic_cast<const UIntSpinBox*> (w);
    if (spin)
        return spin->value();
    else
    {
        kWarning() << "could not cast widget";
        return QVariant();
    }
}

template<typename T>
void UnsignedDataInformation<T>::setWidgetData(QWidget* w) const
{
    UIntSpinBox* spin = dynamic_cast<UIntSpinBox*> (w);
    if (spin)
        spin->setValue(mValue);
}

template<typename T>
UnsignedDataInformation<T>::UnsignedDataInformation(QString name, DataInformation* parent)
        : PrimitiveDataInformation(name, parent), mValue(0)
{
}

template<typename T>
UnsignedDataInformation<T>::UnsignedDataInformation(const UnsignedDataInformation& d)
        : PrimitiveDataInformation(d), mValue(d.mValue)
{
}

template<typename T>
AllPrimitiveTypes UnsignedDataInformation<T>::qVariantToAllPrimitiveTypes(const QVariant& value) const
{
    if (!value.isValid())
        kDebug() << "invalid QVariant passed.";

    //This is fine since all the values are unsigned
    return AllPrimitiveTypes(value.toULongLong());
}

template<typename T>
inline int UnsignedDataInformation<T>::displayBase()
{
    int base = Kasten::StructViewPreferences::unsignedDisplayBase();
    if (base == Kasten::StructViewPreferences::EnumUnsignedDisplayBase::Binary)
        return 2;
    else if (base == Kasten::StructViewPreferences::EnumUnsignedDisplayBase::Hexadecimal)
        return 16;
    else
        return 10; //safe default value
}

template<typename T>
AllPrimitiveTypes UnsignedDataInformation<T>::value() const
{
    return AllPrimitiveTypes(mValue);
}

template<typename T>
void UnsignedDataInformation<T>::setValue(AllPrimitiveTypes newVal)
{
    mValue = newVal.ulongValue; //This is safe since the value is unsigned
}

template<typename T>
int UnsignedDataInformation<T>::size() const
{
    return sizeof(T) * 8;
}

#endif // UNSIGNEDDATAINFORMATION_H
