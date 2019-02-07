/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KASTEN_IF_ZOOMABLE_HPP
#define KASTEN_IF_ZOOMABLE_HPP

// Qt
#include <QtPlugin>

namespace Kasten {
namespace If {

// TODO: add support for automatic zoom
// enum { Manual, Width, Height, FullSize };
// perhaps zoomLevelChanged needs a flag automatically
class Zoomable
{
public:
    virtual ~Zoomable();

public: // set/action
    virtual void setZoomLevel(double Level) = 0;

public: // get
    virtual double zoomLevel() const = 0;

public: // signal
    virtual void zoomLevelChanged(double Level) = 0;
};

inline Zoomable::~Zoomable() = default;

}
}

Q_DECLARE_INTERFACE(Kasten::If::Zoomable, "org.kde.kasten.if.zoomable/1.0")

#endif