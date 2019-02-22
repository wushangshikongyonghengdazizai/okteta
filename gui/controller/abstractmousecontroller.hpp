/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_ABSTRACTMOUSECONTROLLER_HPP
#define OKTETA_ABSTRACTMOUSECONTROLLER_HPP

class QMouseEvent;

namespace Okteta {

class AbstractByteArrayView;

class AbstractMouseController
{
protected:
    AbstractMouseController(AbstractByteArrayView* view, AbstractMouseController* parent);

public:
    AbstractMouseController() = delete;
    virtual ~AbstractMouseController();

public: // API to be implemented
    virtual bool handleMousePressEvent(QMouseEvent* mouseEvent);
    virtual bool handleMouseMoveEvent(QMouseEvent* mouseEvent);
    virtual bool handleMouseReleaseEvent(QMouseEvent* mouseEvent);
    virtual bool handleMouseDoubleClickEvent(QMouseEvent* mouseEvent);

protected:
    AbstractMouseController* mParent;
    AbstractByteArrayView* mView;
};

}

#endif
