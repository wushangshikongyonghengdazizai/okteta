/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UINTSPINBOX_H
#define UINTSPINBOX_H

// Qt
#include <QtGui/QAbstractSpinBox>
// C++
#include <limits>


class UIntSpinBox : public QAbstractSpinBox
{
  public:
    explicit UIntSpinBox( QWidget* parent = 0 );

    virtual ~UIntSpinBox();

  public:
    quint64 value() const;

    quint64 maximum() const;

  public:
    void setValue( quint64 value );

    void setAsHex( bool asHex );

    void setMaximum( quint64 max );

  protected: // QAbstractSpinBox API
    virtual QValidator::State validate( QString& input, int& pos ) const;
    virtual void stepBy( int steps );
    virtual void fixup( QString& input ) const;
    virtual StepEnabled stepEnabled () const;

  protected:
    void updateEditLine() const;

  protected:
    QString mPrefix;
    int mBase;

    mutable quint64 mValue;

    quint64 mMaximum;
};


inline UIntSpinBox::UIntSpinBox( QWidget *parent )
  : QAbstractSpinBox( parent ),
    mBase( 10 ),
    mValue( 0 ),
    mMaximum( std::numeric_limits<quint64>::max() )
{
}

inline quint64 UIntSpinBox::value()   const { return mValue; }
inline quint64 UIntSpinBox::maximum() const { return mMaximum; }

inline void UIntSpinBox::setMaximum( quint64 maximum )
{
    if( mMaximum == maximum )
        return;

    mMaximum = maximum;

    if( mValue > mMaximum )
    {
        mValue = mMaximum;

        updateEditLine();
    }
}

inline void UIntSpinBox::setValue( quint64 value )
{
    if( value > mMaximum )
        value = mMaximum;

    if( mValue == value )
        return;

    mValue = value;

    updateEditLine();
}

inline void UIntSpinBox::setAsHex( bool asHex )
{
    mBase = asHex ? 16 : 10;
    mPrefix = asHex ? QString::fromLatin1( "0x" ) : QString();
}

inline UIntSpinBox::~UIntSpinBox() {}

#endif