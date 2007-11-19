/***************************************************************************
                          testdocumentfilesynchronizer.h  -  description
                             -------------------
    begin                : Fri Nov 16 2007
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


#ifndef TESTDOCUMENTFILESYNCHRONIZER_H
#define TESTDOCUMENTFILESYNCHRONIZER_H

// kakao
#include <kabstractdocumentfilesystemsynchronizer.h>
// Qt
#include <QtCore/QByteArray>

class TestDocument;

class TestDocumentFileSynchronizer : public KAbstractDocumentFileSystemSynchronizer
{
    Q_OBJECT

  public:
    explicit TestDocumentFileSynchronizer( const KUrl &originUrl, const QByteArray &header = QByteArray() );
    TestDocumentFileSynchronizer( KAbstractDocument *document, const KUrl &url,
                                  KAbstractDocumentSynchronizer::ConnectOption option,
                                  const QByteArray &header = QByteArray() );

  public: // KAbstractDocumentSynchronizer API
    virtual KAbstractDocument *document() const;

  protected: // KAbstractDocumentFileSystemSynchronizer API
    virtual KAbstractDocument *loadFromFile( const QString &tmpFileName );
    virtual bool reloadFromFile( const QString &localFilePath );
    virtual bool writeToFile( const QString &localFilePath );
    virtual bool syncWithFile( const QString &localFilePath,
                               KAbstractDocumentSynchronizer::ConnectOption option );

  protected:
    TestDocument *mDocument;
    QByteArray mHeader;
};

#endif
