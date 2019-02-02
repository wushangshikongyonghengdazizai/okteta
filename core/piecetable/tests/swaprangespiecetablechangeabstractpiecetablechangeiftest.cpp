/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "swaprangespiecetablechangeabstractpiecetablechangeiftest.hpp"

// test object
#include <piecetable/swaprangespiecetablechange.hpp>
// lib
#include <piecetable/piecetable.hpp>
// Qt
#include <QTest>

namespace KPieceTable {

static const Address FirstRangeStart = 30;
static const Address SecondRangeStart = 50;
static const Address SecondRangeEnd = 66;

AbstractPieceTableChange* SwapRangesPieceTableChangeAbstractPieceTableChangeIfTest::createPieceTableChange()
{
    SwapRangesPieceTableChange* pieceTableChange =
        new SwapRangesPieceTableChange(FirstRangeStart, AddressRange(SecondRangeStart, SecondRangeEnd));

    return pieceTableChange;
}

void SwapRangesPieceTableChangeAbstractPieceTableChangeIfTest::changePieceTable(PieceTable* pieceTable)
{
    pieceTable->swap(FirstRangeStart, AddressRange(SecondRangeStart, SecondRangeEnd));
}

void SwapRangesPieceTableChangeAbstractPieceTableChangeIfTest::deletePieceTableChange(AbstractPieceTableChange* pieceTableChange)
{
    delete pieceTableChange;
}

}

QTEST_GUILESS_MAIN(KPieceTable::SwapRangesPieceTableChangeAbstractPieceTableChangeIfTest)
