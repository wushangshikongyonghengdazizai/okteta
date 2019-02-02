/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010 Alex Richardson <alex.richardson@gmx.de>
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

#include "structureviewitemdelegate.hpp"
#include "datatypes/datainformation.hpp"

#include <QScopedPointer>

StructureViewItemDelegate::StructureViewItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

StructureViewItemDelegate::~StructureViewItemDelegate() = default;

QWidget* StructureViewItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                                                 const QModelIndex& index) const
{
    Q_UNUSED(option)
    if (!index.isValid()) {
        return nullptr;
    }
    DataInformationBase* dataB = static_cast<DataInformationBase*> (index.internalPointer());
    if (!dataB || dataB->isTopLevel()) {
        return nullptr;
    }
    DataInformation* data = dataB->asDataInformation();
    QWidget* ret = data->createEditWidget(parent);
    ret->setFocusPolicy(Qt::WheelFocus);
    return ret;
}

void StructureViewItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                             const QModelIndex& index) const
{
    if (!index.isValid()) {
        return;
    }
    DataInformationBase* dataB = static_cast<DataInformationBase*> (index.internalPointer());
    if (!dataB || dataB->isTopLevel()) {
        return;
    }
    DataInformation* data = dataB->asDataInformation();
    QVariant value = data->dataFromWidget(editor);
    model->setData(index, value, Qt::EditRole);
}

void StructureViewItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{

    if (!index.isValid()) {
        return;
    }
    DataInformationBase* dataB = static_cast<DataInformationBase*> (index.internalPointer());
    if (!dataB || dataB->isTopLevel()) {
        return;
    }
    DataInformation* data = dataB->asDataInformation();
    data->setWidgetData(editor);
}

QSize StructureViewItemDelegate::sizeHint(const QStyleOptionViewItem& option,
                                          const QModelIndex& index) const
{
    return QStyledItemDelegate::sizeHint(option, index) + QSize(0, 8);
}
