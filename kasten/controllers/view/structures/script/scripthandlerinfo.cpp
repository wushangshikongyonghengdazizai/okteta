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

#include "scripthandlerinfo.hpp"
#include "classes/arrayscriptclass.hpp"
#include "classes/primitivescriptclass.hpp"
#include "classes/structunionscriptclass.hpp"
#include "classes/stringscriptclass.hpp"
#include "classes/enumscriptclass.hpp"
#include "classes/bitfieldscriptclass.hpp"
#include "classes/pointerscriptclass.hpp"

ScriptHandlerInfo::ScriptHandlerInfo(QScriptEngine* engine, ScriptLogger* logger)
    : mArrayClass(new ArrayScriptClass(engine, this))
    , mPrimitiveClass(new PrimitiveScriptClass(engine, this))
    , mEnumClass(new EnumScriptClass(engine, this))
    , mStructUnionClass(new StructUnionScriptClass(engine, this))
    , mStringClass(new StringScriptClass(engine, this))
    , mBitfieldClass(new BitfieldScriptClass(engine, this))
    , mPointerClass(new PointerScriptClass(engine, this))
    , mLogger(logger)
{
    Q_CHECK_PTR(mLogger);
}

ScriptHandlerInfo::~ScriptHandlerInfo() = default;

ScriptHandlerInfo::Mode ScriptHandlerInfo::mode() const
{
    return mMode;
}

void ScriptHandlerInfo::setMode(Mode m)
{
    mMode = m;
}

ScriptLogger* ScriptHandlerInfo::logger() const
{
    return mLogger;
}
