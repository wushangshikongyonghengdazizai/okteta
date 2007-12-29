/***************************************************************************
                          headerfooterframerenderer.cpp  -  description
                             -------------------
    begin                : Tue Dec 11 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/

#include "headerfooterframerenderer.h"

// KDE
#include <KLocale>
#include <KGlobal>
#include <KUser>
// Qt
#include <QtCore/QHash>
#include <QtCore/QDateTime>
#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>


HeaderFooterFrameRenderer::HeaderFooterFrameRenderer( const PrintInfo* info )
 : mInfo( info ),
   mHeight( 0 ), mWidth( 0 ), mBoxStyle( NoBox ), mLineWidth( 1 ), mBoxMargin( 6 ),
   mBgColor( Qt::lightGray ), mFgColor( Qt::black ), mGloballyReplacedTextList()
{
    mOriginalTextList << QString() << QString() << QString();
    calculateHeight();
}

int HeaderFooterFrameRenderer::height() const { return mHeight; }
int HeaderFooterFrameRenderer::width() const { return mWidth; }

void HeaderFooterFrameRenderer::setWidth( int width ) { mWidth = width; }

void HeaderFooterFrameRenderer::setTexts( const QString &leftText, const QString &centerText, const QString &rightText )
{
    mOriginalTextList.clear();
    mOriginalTextList << leftText << centerText << rightText;
}

void HeaderFooterFrameRenderer::setBoxStyle( int boxStyle )
{
    if( mBoxStyle == boxStyle )
        return;
    mBoxStyle = boxStyle;
    calculateHeight();
}

void HeaderFooterFrameRenderer::calculateHeight()
{
    const QFontMetrics fontMetrics( mFont );

    mHeight = fontMetrics.height();

//     const bool hasSpaceAbove = ( mBoxStyle & (BackgroundDrawn|LineAbove) );
//     const bool hasSpacedBelow = ( mBoxStyle & (BackgroundDrawn|LineBelow) );
//     if( !(hasSpaceAbove||hasSpacedBelow) )
        mHeight += fontMetrics.leading();
//     else
//     {
//         if( hasSpaceAbove )
//         {
//         mHeight += mBoxMargin * 2;
// 
//         }
//     }
}

void HeaderFooterFrameRenderer::prepare()
{
    const QDateTime dateTime = QDateTime::currentDateTime();
    const KUser user( KUser::UseRealUserID );
    const KUrl url = mInfo->url();

    // create list of replacements
    QHash<QChar,QString> tagReplacements;

    tagReplacements['d'] = KGlobal::locale()->formatDateTime( dateTime, KLocale::ShortDate );
    tagReplacements['D'] = KGlobal::locale()->formatDateTime( dateTime, KLocale::LongDate );
    tagReplacements['h'] = KGlobal::locale()->formatTime( dateTime.time(), false );
    tagReplacements['y'] = KGlobal::locale()->formatDate( dateTime.date(), KLocale::ShortDate );
    tagReplacements['Y'] = KGlobal::locale()->formatDate( dateTime.date(), KLocale::LongDate );
    tagReplacements['u'] = user.loginName();
    tagReplacements['U'] = user.fullName();
//     tagReplacements['f'] = isSelection ? i18n("(Selection of) %1", url.fileName()) : url.fileName();
    tagReplacements['f'] = url.fileName();
    tagReplacements['F'] = url.prettyUrl();
    tagReplacements['P'] = QString::number( mInfo->noOfPages() );

    // create text with globally replaced tags
    const int sizeOfTag = 2;
    QRegExp tagsPattern( "%([dDhyYuUfFP])" );

    mGloballyReplacedTextList.clear();

    for( int i=0; i<3; ++i )
    {
        QString text = mOriginalTextList.at( i );
        int pos = 0;
        while( true )
        {
            pos = tagsPattern.indexIn( text, pos );
            if( pos < 0 )
                break;
            QString replacement = tagReplacements[text[pos+1]];
            text.replace( (uint)pos, sizeOfTag, replacement );
            pos += replacement.length();
        }
        mGloballyReplacedTextList << text;
    }
}

void HeaderFooterFrameRenderer::renderFrame( QPainter *painter, int frameIndex )
{
    painter->setPen( mFgColor );
    painter->setFont( mFont );
//     if( mBoxStyle & BackgroundDrawn )
//         painter->fillRect( 0, 0, mWidth, mHeight, mBgColor );

    const bool isTextFramed = false;//( mBoxStyle & (Box|BackgroundDrawn) );

    const int horizontalAlign[3] = { Qt::AlignLeft, Qt::AlignHCenter, Qt::AlignRight };
    int verticalAlign = isTextFramed ? Qt::AlignVCenter : Qt::AlignTop;
    int margin = 0;//( mBoxStyle & (mBoxDrawn || mBackgroundDrawn ) ? mBoxMargin : 0;
//     if ( mBoxStyle & LinesAtSide )
//         margin += mLineWidth;
    const int rightEnd = mWidth - (margin*2);

    for( int i=0; i<3; i++ )
    {
        QString text = mGloballyReplacedTextList[i];
        // substitute locally
        const char pageNumberTag[] = "%p";
        if( text.indexOf(pageNumberTag) != -1 )
            text.replace( pageNumberTag, QString::number(frameIndex+1) ); //TODO: frameIndex != pageNumber in general
        int align = verticalAlign | horizontalAlign[i];

        painter->drawText( margin, 0, rightEnd, mHeight, align, text );
    }
//     if ( !isTextFramed )
    {
//         painter->drawLine( 0, mHeight-mLineWidth, mWidth, mHeight-mLineWidth );
//         painter->drawLine( 0, maxHeight + mBoxMargin - 1, headerWidth, maxHeight + mBoxMargin - 1 );
//         painter->drawRect( 0, 0, pdmWidth, pdmHeight);
//             paint.drawLine(0, headerHeight, headerWidth, headerHeight);
    }
}

HeaderFooterFrameRenderer::~HeaderFooterFrameRenderer() {}
