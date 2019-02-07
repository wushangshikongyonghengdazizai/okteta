/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BOOKMARKSCONTROLLER_HPP
#define BOOKMARKSCONTROLLER_HPP

// lib
#include <kasten/okteta/oktetakastencontrollers_export.hpp>
// Kasten gui
#include <Kasten/AbstractXmlGuiController>
// Okteta core
#include <Okteta/Address>
// Qt
#include <QList>

namespace Okteta {
class Bookmarkable;
class Bookmark;
class AbstractByteArrayModel;
}
class KXMLGUIClient;
class KAction;
class QActionGroup;
class QAction;

namespace Kasten {

class ByteArrayView;

class OKTETAKASTENCONTROLLERS_EXPORT BookmarksController : public AbstractXmlGuiController
{
    Q_OBJECT

private:
    static const int MaxEntryLength = 150;
    static const int MaxBookmarkNameSize = 40;

public:
    explicit BookmarksController(KXMLGUIClient* guiClient);
    ~BookmarksController() override;

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // action slots
    void createBookmark();
    void deleteBookmark();
    void deleteAllBookmarks();
    void gotoNextBookmark();
    void gotoPreviousBookmark();

    void updateBookmarks();
    void onBookmarkTriggered(QAction*);

private Q_SLOTS:
    void onBookmarksAdded(const QVector<Okteta::Bookmark>& bookmarks);
    void onBookmarksRemoved(const QVector<Okteta::Bookmark>& bookmarks);
    void onCursorPositionChanged(Okteta::Address newPosition);

private:
    KXMLGUIClient* mGuiClient;

    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArray = nullptr;
    Okteta::Bookmarkable* mBookmarks = nullptr;

    QAction* mCreateAction;
    QAction* mDeleteAction;
    QAction* mDeleteAllAction;

    QAction* mGotoNextBookmarkAction;
    QAction* mGotoPreviousBookmarkAction;

    QActionGroup* mBookmarksActionGroup;
};

}

#endif