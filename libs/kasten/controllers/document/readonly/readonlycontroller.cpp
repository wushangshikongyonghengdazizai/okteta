/*
    This file is part of the Kasten Framework, made within the KDE community.

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

#include "readonlycontroller.h"

// Kasten core
#include <kasten/abstractdocument.h>
// KF5
#include <KXMLGUIClient>
#include <KLocalizedString>
#include <KActionCollection>
#include <KToggleAction>
#include <KGuiItem>

namespace Kasten {

ReadOnlyController::ReadOnlyController(KXMLGUIClient* guiClient)
    : mDocument(nullptr)
{
    mSetReadOnlyAction = new KToggleAction(QIcon::fromTheme(QStringLiteral("object-unlocked")),
                                           i18nc("@option:check set the document to read-only", "Set Read-only"),
                                           this);
    const QString checkedText = i18nc("@option:check set the document to read-write", "Set Read-write");
    const KGuiItem checkedState(checkedText, QIcon::fromTheme(QStringLiteral("object-locked")));
    mSetReadOnlyAction->setCheckedState(checkedState);
    connect(mSetReadOnlyAction, &QAction::triggered,
            this, &ReadOnlyController::setReadOnly);

    guiClient->actionCollection()->addAction(QStringLiteral("isreadonly"), mSetReadOnlyAction);

    setTargetModel(nullptr);
}

void ReadOnlyController::setTargetModel(AbstractModel* model)
{
    if (mDocument) {
        mDocument->disconnect(mSetReadOnlyAction);
    }

    mDocument = model ? model->findBaseModel<AbstractDocument*>() : nullptr;

    if (mDocument) {
        mSetReadOnlyAction->setChecked(mDocument->isReadOnly());

        connect(mDocument, &AbstractModel::readOnlyChanged,
                mSetReadOnlyAction, &QAction::setChecked);
        connect(mDocument, &AbstractModel::modifiableChanged,
                mSetReadOnlyAction, &QAction::setEnabled);
    }

    mSetReadOnlyAction->setEnabled(mDocument ? mDocument->isModifiable() : false);
}

void ReadOnlyController::setReadOnly(bool isReadOnly)
{
    mDocument->setReadOnly(isReadOnly);
}

}
