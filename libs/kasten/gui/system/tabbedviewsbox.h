/*
    This file is part of the Kasten Framework, part of the KDE project.

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

#ifndef TABBEDVIEWSBOX_H
#define TABBEDVIEWSBOX_H

// Qt
#include <QtGui/QWidget>

class KTabWidget;
class QShortcut;


namespace Kasten
{


class TabbedViewsBox : public QWidget
{
    Q_OBJECT

  public:
    enum Area { TopArea, BottomArea };

  public:
    explicit TabbedViewsBox( QWidget* parent = 0 );
    virtual ~TabbedViewsBox();

  public:
    KTabWidget* tabWidget() const;
    QWidget* bottomWidget() const;

  public:
    void setBottomWidget( QWidget* bottomWidget );

  protected Q_SLOTS:
    void onDone();

  protected:
    KTabWidget* mTabWidget;

    QWidget* mBottomWidget;
    QShortcut* mEscapeShortcut;
};

}

#endif