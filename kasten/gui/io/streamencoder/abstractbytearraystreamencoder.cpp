/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "abstractbytearraystreamencoder.hpp"

// lib
#include <bytearrayview.hpp>
#include <kasten/okteta/bytearraydocument.hpp>
#include <kasten/okteta/bytearrayselection.hpp>
// Okteta core
#include <okteta/abstractbytearraymodel.hpp>
// Qt
#include <QBuffer>

namespace Kasten {

static const int MaxPreviewSize = 100;

AbstractByteArrayStreamEncoder::AbstractByteArrayStreamEncoder(const QString& remoteTypeName,
                                                               const QString& remoteMimeType)
    : AbstractModelStreamEncoder(remoteTypeName, remoteMimeType, QStringLiteral("text/plain"))
{}

AbstractByteArrayStreamEncoder::~AbstractByteArrayStreamEncoder() = default;

QString AbstractByteArrayStreamEncoder::modelTypeName(AbstractModel* model, const AbstractModelSelection* selection) const
{
    Q_UNUSED(selection)

    const ByteArrayDocument * byteArrayDocument = model->findBaseModel<const ByteArrayDocument*>();

    return byteArrayDocument ? byteArrayDocument->typeName() : QString();
}

bool AbstractByteArrayStreamEncoder::encodeToStream(QIODevice* device,
                                                    AbstractModel* model, const AbstractModelSelection* selection)
{
    const ByteArrayView* byteArrayView = qobject_cast<const ByteArrayView*>(model);

    const ByteArrayDocument* byteArrayDocument =
        byteArrayView ? qobject_cast<const ByteArrayDocument*>(byteArrayView->baseModel()) : nullptr;
    if (!byteArrayDocument) {
        return false;
    }

    const Okteta::AbstractByteArrayModel* byteArray = byteArrayDocument->content();

    const ByteArraySelection* byteArraySelection =
        selection ? static_cast<const ByteArraySelection*>(selection) : nullptr;

    const Okteta::AddressRange range = byteArraySelection && byteArraySelection->isValid() ?
                                       byteArraySelection->range() :
                                       Okteta::AddressRange::fromWidth(0, byteArray->size());

    const bool success = encodeDataToStream(device, byteArrayView, byteArray, range);

    return success;
}

QString AbstractByteArrayStreamEncoder::previewData(AbstractModel* model, const AbstractModelSelection* selection)
{
    const ByteArrayView* byteArrayView = qobject_cast<const ByteArrayView*>(model);

    const ByteArrayDocument* byteArrayDocument =
        byteArrayView ? qobject_cast<const ByteArrayDocument*>(byteArrayView->baseModel()) : nullptr;
    if (!byteArrayDocument) {
        return {};
    }

    const Okteta::AbstractByteArrayModel* byteArray = byteArrayDocument->content();

    const ByteArraySelection* byteArraySelection =
        selection ? static_cast<const ByteArraySelection*>(selection) : nullptr;

    Okteta::AddressRange range = byteArraySelection && byteArraySelection->isValid() ?
                                 byteArraySelection->range() :
                                 Okteta::AddressRange::fromWidth(0, byteArray->size());
    range.restrictEndByWidth(MaxPreviewSize);

    QByteArray data;
    QBuffer dataBuffer(&data);
    dataBuffer.open(QIODevice::WriteOnly);

    const bool success = encodeDataToStream(&dataBuffer, byteArrayView, byteArray, range);
    dataBuffer.close();

    return success ? QString::fromLatin1(data) : QString();
}

}
