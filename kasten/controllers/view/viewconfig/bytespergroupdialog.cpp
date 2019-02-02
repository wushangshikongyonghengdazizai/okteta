/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010,2014 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytespergroupdialog.hpp"

// KF5
#include <KLocalizedString>
// Qt
#include <QFormLayout>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QDialogButtonBox>

namespace Kasten {

BytesPerGroupDialog::BytesPerGroupDialog(QWidget* parent)
    : QDialog(parent)
{
    QFormLayout* pageLayout = new QFormLayout();

    mGroupedBytesCountEdit = new QSpinBox(this);
    mGroupedBytesCountEdit->setRange(0, INT_MAX);
    const QString noGroupingText = i18nc("@label",
                                         "No grouping.");
    mGroupedBytesCountEdit->setSpecialValueText(noGroupingText);
    const QString groupedBytesCountLabel =
        i18nc("@label:spinbox number of bytes which are grouped",
              "Bytes per Group:");
    pageLayout->addRow(groupedBytesCountLabel, mGroupedBytesCountEdit);

    QDialogButtonBox* dialogButtonBox = new QDialogButtonBox;
    dialogButtonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout* layout = new QVBoxLayout;

    layout->addLayout(pageLayout);
    layout->addWidget(dialogButtonBox);
    setLayout(layout);

    const QString caption =
        i18nc("@title:window", "Bytes per Group");
    setWindowTitle(caption);
}

BytesPerGroupDialog::~BytesPerGroupDialog() = default;

int BytesPerGroupDialog::groupedBytesCount() const { return mGroupedBytesCountEdit->value(); }

void BytesPerGroupDialog::setGroupedBytesCount(int groupedBytesCount)
{
    mGroupedBytesCountEdit->setValue(groupedBytesCount);
}

}
