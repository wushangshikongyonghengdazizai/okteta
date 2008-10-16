/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#include "viewstatuscontroller.h"

// lib
#include <kbytearraydisplay.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <khe.h>
// KDE
#include <KStatusBar>
#include <KLocale>
// Qt
#include <QtGui/QLabel>


ViewStatusController::ViewStatusController( KStatusBar* statusBar )
 : mByteArrayView( 0 )
{
    mPrintFunction = KHEUI::KOffsetFormat::printFunction( KHEUI::KOffsetFormat::Hexadecimal );

    mOffsetLabel = new QLabel( statusBar );
    statusBar->addWidget( mOffsetLabel, 0 );

    mOverwriteModeLabel = new QLabel( statusBar );
    statusBar->addWidget( mOverwriteModeLabel, 0 );

    mValueCodingLabel = new QLabel( statusBar );
    mValueCodingLabel->setToolTip(
        i18nc("@info:tooltip","Coding in the value column of the current view.") );
    statusBar->addWidget( mValueCodingLabel, 0 );

    mCharCodingLabel = new QLabel( statusBar );
    mCharCodingLabel->setToolTip(
        i18nc("@info:tooltip","Encoding in the character column of the current view.") );
    statusBar->addWidget( mCharCodingLabel, 0 );

    setTargetModel( 0 );
}

void ViewStatusController::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView ) mByteArrayView->disconnect( this );

    KByteArrayDisplay* view = model ? model->findBaseModel<KByteArrayDisplay*>() : 0;
    mByteArrayView = view ? qobject_cast<KHEUI::KByteArrayView*>( view->widget() ) : 0;

    const bool hasView = ( mByteArrayView != 0 );
    if( hasView )
    {
        mStartOffset = mByteArrayView->startOffset();

        onCursorPositionChanged( mByteArrayView->cursorPosition() );
        onOverwriteModeChanged( mByteArrayView->isOverwriteMode() );
        onValueCodingChanged( (int)mByteArrayView->coding() );
        onCharCodecChanged( mByteArrayView->encodingName() );

        connect( mByteArrayView, SIGNAL(cursorPositionChanged( int )), SLOT(onCursorPositionChanged( int )) );
        connect( mByteArrayView, SIGNAL(overwriteModeChanged( bool )), SLOT(onOverwriteModeChanged( bool )) );
        connect( mByteArrayView, SIGNAL(valueCodingChanged( int )), SLOT(onValueCodingChanged( int )) );
        connect( mByteArrayView, SIGNAL(charCodecChanged( const QString& )),
            SLOT(onCharCodecChanged( const QString& )) );
    }
    else
    {
        const QString emptyString( '-' );

        mOffsetLabel->setText( i18nc("@info:status offset value not available", "Offset: -") );
        mOverwriteModeLabel->setText( emptyString );
        mValueCodingLabel->setText( emptyString );
        mCharCodingLabel->setText( emptyString );
    }

    mOffsetLabel->setEnabled( hasView );
    mOverwriteModeLabel->setEnabled( hasView );
    mValueCodingLabel->setEnabled( hasView );
    mCharCodingLabel->setEnabled( hasView );
}


void ViewStatusController::onCursorPositionChanged( int offset )
{
    static char codedOffset[KHEUI::KOffsetFormat::MaxFormatWidth+1];

    mPrintFunction( codedOffset, mStartOffset + offset );

    mOffsetLabel->setText( i18n("Offset: %1", QLatin1String(codedOffset)) );
}

void ViewStatusController::onOverwriteModeChanged( bool isOverwrite )
{
    const QString overwriteModeText = isOverwrite ?
        i18nc( "@info:status short for: Overwrite mode", "OVR" ) :
        i18nc( "@info:status short for: Insert mode",    "INS");
    mOverwriteModeLabel->setText( overwriteModeText );

    const QString overwriteModeToolTip = isOverwrite ?
        i18nc( "@info:tooltip", "Overwrite mode" ) :
        i18nc( "@info:tooltip", "Insert mode" );
    mOverwriteModeLabel->setToolTip( overwriteModeToolTip );
}

void ViewStatusController::onValueCodingChanged( int coding )
{
    const QString valueCodingName =
         coding == KHECore::HexadecimalCoding ?
            i18nc("@info:status encoding of the bytes as values in the hexadecimal format","Hexadecimal" ) :
         coding == KHECore::DecimalCoding ?
            i18nc("@info:status encoding of the bytes as values in the decimal format",    "Decimal") :
         coding == KHECore::OctalCoding ?
            i18nc("@info:status encoding of the bytes as values in the octal format",      "Octal" ) :
         coding == KHECore::BinaryCoding ?
            i18nc("@info:status encoding of the bytes as values in the binary format",     "Binary") :
            QString();

    mValueCodingLabel->setText( valueCodingName );
}

void ViewStatusController::onCharCodecChanged( const QString& charCodecName )
{
    mCharCodingLabel->setText( charCodecName );
}
