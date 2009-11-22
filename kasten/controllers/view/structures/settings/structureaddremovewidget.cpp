/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
 *
 *   Copyright 2009 Alex Richardson <alex.richardson@gmx.de>
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
#include "structureaddremovewidget.h"
#include "structviewpreferences.h"
#include "../structtool.h"
#include "../structuredefinitionfile.h"

#include <KArrowButton>
#include <KPushButton>
#include <QLabel>
#include <QLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QRegExp>
#include <KLocale>
#include <KDebug>
#include <KConfigDialogManager>

using namespace Kasten;
StructureAddRemoveWidget::StructureAddRemoveWidget(Kasten::StructTool* tool,
        QWidget* parent) :
    QWidget(parent)
{
    KConfigDialogManager::changedMap()->insert("StructureAddRemoveWidget",
            SIGNAL(changed(const QStringList&)));
    QHBoxLayout* baseLayout;
    QVBoxLayout* tree1Layout;
    QVBoxLayout* tree2Layout;
    QVBoxLayout* leftRightLayout;
    QVBoxLayout* upDownLayout;

    baseLayout = new QHBoxLayout();

    tree1Layout = new QVBoxLayout();
    mTree1Label = new QLabel(i18nc("@info:label", "Available structures:"), this);
    tree1Layout->addWidget(mTree1Label);
    mTreeAvailable = new QTreeWidget(this);
    mTreeAvailable->setHeaderHidden(true);
    mTreeAvailable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mTreeAvailable->setColumnCount(2);
    mTreeAvailable->setColumnHidden(1, true);
    tree1Layout->addWidget(mTreeAvailable);

    tree2Layout = new QVBoxLayout();
    mTree2Label = new QLabel(i18nc("@info:label", "Selected structures:"), this);
    tree2Layout->addWidget(mTree2Label);
    mTreeSelected = new QTreeWidget(this);
    mTreeSelected->setHeaderHidden(true);
    mTreeSelected->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mTreeSelected->setColumnCount(2);
    mTreeSelected->setColumnHidden(1, true);
    tree2Layout->addWidget(mTreeSelected);

    leftRightLayout = new QVBoxLayout();
    leftRightLayout->addStretch();
    mRightButton = new KPushButton(KIcon("arrow-right"), "", this);
    leftRightLayout->addWidget(mRightButton);
    mLeftButton = new KPushButton(KIcon("arrow-left"), "", this);
    leftRightLayout->addWidget(mLeftButton);
    leftRightLayout->addStretch();

    upDownLayout = new QVBoxLayout();
    upDownLayout->addStretch();
    mUpButton = new KPushButton(KIcon("arrow-up"), "", this);
    upDownLayout->addWidget(mUpButton);
    mDownButton = new KPushButton(KIcon("arrow-down"), "", this);
    upDownLayout->addWidget(mDownButton);
    upDownLayout->addStretch();

    baseLayout->addLayout(tree1Layout);
    baseLayout->addLayout(leftRightLayout);
    baseLayout->addLayout(tree2Layout);
    baseLayout->addLayout(upDownLayout);
    setLayout(baseLayout);

    connect(mLeftButton, SIGNAL(pressed()), SLOT(moveLeft()));
    connect(mRightButton, SIGNAL(pressed()), SLOT(moveRight()));
    connect(mUpButton, SIGNAL(pressed()), SLOT(moveUp()));
    connect(mDownButton, SIGNAL(pressed()), SLOT(moveDown()));

    const QList<StructureDefinitionFile*> loadedDefs = tool->loadedDefs();
    QList<QTreeWidgetItem*> availableItems;
    foreach(const StructureDefinitionFile* def,loadedDefs)
        {
            QString relPath = StructTool::defsDir.relativeFilePath(
                    def->path().absoluteFilePath());
            QTreeWidgetItem* item = new QTreeWidgetItem(mTreeAvailable,
                    QStringList() << def->title() << relPath);
            foreach(const DataInformation* data,def->structures())
                {
                    QTreeWidgetItem* subItem = new QTreeWidgetItem(item,
                            QStringList() << data->getName() << relPath);
                    item->addChild(subItem);
                }
            availableItems.append(item);
        }

    //already loaded defs:
    QRegExp regex("'(.+)':'(.+)'");
    QStringList loadedStructs = StructViewPreferences::loadedStructures();
    foreach(QString s,loadedStructs)
        {
            int pos = regex.indexIn(s);
            if (pos > -1)
            {
                QString path = regex.cap(1);
                QString structName = regex.cap(2);
//                kDebug() << "path=" << path << " name=" << structName;
                QTreeWidgetItem* item = new QTreeWidgetItem(mTreeSelected,
                        QStringList() << structName << path);
                mTreeSelected->addTopLevelItem(item);
                // ...
            }
        }
    mTreeAvailable->addTopLevelItems(availableItems);
    syncData();
}

StructureAddRemoveWidget::~StructureAddRemoveWidget()
{
    delete mTree1Label;
    delete mTreeAvailable;

    delete mTree2Label;
    delete mTreeSelected;

    delete mRightButton;
    delete mLeftButton;

    delete mUpButton;
    delete mDownButton;
}

void StructureAddRemoveWidget::moveLeft()
{
    QList<QTreeWidgetItem*> selected = mTreeSelected->selectedItems();
    bool changed = false;
    foreach(QTreeWidgetItem* item,selected)
        {
            delete mTreeSelected->takeTopLevelItem(
                    mTreeSelected->indexOfTopLevelItem(item));
            changed = true;
        }
    if (changed)
        syncData();
}
void StructureAddRemoveWidget::moveRight()
{
    QList<QTreeWidgetItem*> selected = mTreeAvailable->selectedItems();
    bool changed = false;
    foreach(QTreeWidgetItem* item,selected)
        {
            if (!item->parent())
                continue; //maybe sometime add all subitems
            QTreeWidgetItem* moveOver = new QTreeWidgetItem(mTreeSelected,
                    QStringList() << item->text(0) << item->parent()->text(1));
            //item name then parent name then path
            mTreeSelected->addTopLevelItem(moveOver);
            changed = true;
        }
    kDebug() << "";
    if (changed)
        syncData();

}
void StructureAddRemoveWidget::moveUp()
{
    kDebug() << "TODO stub";

}
void StructureAddRemoveWidget::moveDown()
{
    kDebug() << "TODO stub";

}
void StructureAddRemoveWidget::syncData()
{
    QStringList strings;
    for (int i = 0; i < mTreeSelected->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem* item = mTreeSelected->topLevelItem(i);
        QString dataStr = QString("\'%1\':\'%2\'").arg(item->text(1)).arg(
                item->text(0));
        strings.append(dataStr);
    }
    kDebug() << "items to set: " << strings;
    mValues = strings;
    kDebug() << "changed: " << mValues;
    emit changed(mValues);
}