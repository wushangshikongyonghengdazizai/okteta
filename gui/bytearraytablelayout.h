/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003,2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_BYTEARRAYTABLELAYOUT_H
#define OKTETA_BYTEARRAYTABLELAYOUT_H

// lib
#include "coordrange.h"
#include "oktetagui_export.h"
// Okteta core
#include <addressrange.h>


// TODO: rename things like startOffset and firstLineOffset, not really descriptive
namespace Okteta
{

/**@short the logical layout of a byte array table for a view
  *
  * Given the values for
  * * number of bytes per line,
  * * a possible offset of the first line displayed,
  * * a possible offset of the displayed bytearray, and
  * * length of the byte array as well as
  * * offset in the byte array
  * * the number of lines per page jump
  * the following values are calculated:
  * * starting line of display,
  * * starting position in this line,
  * * final line of display,
  * * final position in this line, and
  * * the total number of lines (is final line +1 or 0)
  *
  * This layout sees the buffer as a continuous stream of byte,
  * thus uses each line after the start from the begin to the end.
  *
  * If the buffer is empty the end coord will be set one pos left to the start coord
  * to easen the cursor handling.
  *
  *@author Friedrich W. H.  Kossebau
  */
class OKTETAGUI_EXPORT ByteArrayTableLayout
{
  public:
    ByteArrayTableLayout( int noOfBytesPerLine, Address firstLineOffset, Address startOffset, Address byteArrayOffset, Size byteArrayLength );
    //ByteArrayTableLayout();

    ~ByteArrayTableLayout();


  public: // given values
    /** */
    Address startOffset() const;
    /** */
    Address firstLineOffset() const;
    /** returns number of bytes per line */
    int noOfBytesPerLine() const;
    /** returns the offset of the start of the displayed byte array section */
    Address byteArrayOffset() const;
    /** returns the length of the displayed byte array section */
    Size length() const;
    /** returns number of lines per visual page */
    int noOfLinesPerPage() const;

  public: // calculated values
    int startLine() const;
    int startLinePosition() const;
    /** returns the coord of the start */
    Coord startCoord() const;

    int finalLine() const;
    int finalLinePosition() const;
    /** returns the coord of the end */
    Coord finalCoord() const;

    Address lastByteArrayOffset() const;
    /** tells how much lines this layout needs (incl. blank leading lines due to mStartOffset and mFirstLineOffset) */
    int noOfLines() const;


  public: // value calculation service
    /** calculates the index of the coord
      * If the coord is before the first coord the first index is returned,
      * if the coord is behind the last coord the last index is returned
      */
    Address indexAtCCoord( const Coord& coord ) const;
    /** calculates the index of the first pos in line.
      * If the line is below the first line the first index is returned,
      * if the line is above the last line the last index is returned
      */
    Address indexAtCFirstLinePosition( int line ) const;
    /** calculates the index of last pos in line
      * If the line is below the first line the first index is returned,
      * if the line is above the last line the last index is returned
      */
    Address indexAtCLastLinePosition( int line ) const;
    /** calculates the line in which index is found
      * If the index is below the first index the first line is returned,
      * if the index is above the last index the last line is returned
      */
    int lineAtCIndex( int index ) const;
    /** calculates the coord in which index is found
      * If the index is below the first index the first coord is returned,
      * if the index is above the last index the last coord is returned
      */
    Coord coordOfCIndex( Address index ) const;

    /** calculates the index of coord. if coord is invalid the behaviour is undefinded */
    Address indexAtCoord( const Coord& coord ) const;
    /** calculates the index of the first pos in line. if line is invalid the behaviour is undefinded */
    Address indexAtFirstLinePosition( int line ) const;
    /** calculates the index of last pos in line. if line is invalid the behaviour is undefinded */
    Address indexAtLastLinePosition( int line ) const;
    /** calculates the line in which index is found. if index is invalid the behaviour is undefinded */
    int lineAtIndex( Address index ) const;
    /** calculates the coord in which index is found. if index is invalid the behaviour is undefinded */
    Coord coordOfIndex( Address index ) const;
    /** calculates the range of coords in which the indizes are found. if indizes are invalid the behaviour is undefinded */
    CoordRange coordRangeOfIndizes( const AddressRange& indizes ) const;

    /** returns the used positions in line */
    KDE::Section linePositions( int line ) const;
    /** returns the first Pos in line. if line is invalid the behaviour is undefinded */
    int firstLinePosition( int line ) const;
    /** returns the last Pos in line. if line is invalid the behaviour is undefinded */
    int lastLinePosition( int line ) const;
    /** returns the valid Pos or the first Pos in line. if coord is invalid the behaviour is undefinded */
    int firstLinePosition( const Coord& coord ) const;
    /** returns the valid Pos or the last Pos in line. if coord is invalid the behaviour is undefinded */
    int lastLinePosition( const Coord& coord ) const;
    /** returns true if the line has content */
    bool hasContent( int line ) const;
    /** returns true if the coord is the first in it's line. if coord is invalid the behaviour is undefinded */
    bool atFirstLinePosition( const Coord& coord ) const;
    /** returns true if the coord is the last in it's line. if coord is invalid the behaviour is undefinded */
    bool atLastLinePosition( const Coord& coord ) const;

    /** returns the index if valid or the nearest valid index */
    int correctIndex( int index ) const;
    /** returns the coord if valid or the nearest valid coord */
    Coord correctCoord( const Coord& coord ) const;


  public: // modification access; return true if changes
     /** sets mStartOffset, returns true if changed */
    bool setStartOffset( Address startOffset );
     /** sets mStartOffset, returns true if changed */
    bool setFirstLineOffset( Address firstLineOffset );
    /** sets number of bytes per line, returns true if changed */
    bool setNoOfBytesPerLine( int noOfBytesPerLine );
    /** sets offset in the data to display, returns true if changed */
    bool setByteArrayOffset( Address byteArrayOffset );
    /** sets length of data to display, returns true if changed */
    bool setLength( Size length );
    /** sets number of lines per page, 1 as default */
    void setNoOfLinesPerPage( int noOfLinesPerPage );


  protected:
    /** calculates the start coord by startoffset and number of bytes per line */
    void calcStart();
    /** calculates the final coord by startoffset, length, and number of bytes per line */
    void calcEnd();


 protected:
    /** how many chars per line */
    int mNoOfBytesPerLine;
    /** starting offset of the first displayed line */
    Address mFirstLineOffset;
    /** starting offset of the displayed bytearray */
    Address mStartOffset;
    /** */
    Address mRelativeStartOffset;
    /** offset in the given bytearray */
    Address mByteArrayOffset;
    /** last offset in the displayed bytearray section */
    Address mLastByteArrayOffset;
    /** number of lines that are moved by page up/down */
    int mNoOfLinesPerPage;

  protected: // calculated values, buffered
    /** */
    CoordRange mCoordRange;
};


inline Address ByteArrayTableLayout::startOffset()         const { return mStartOffset; }
inline Address ByteArrayTableLayout::firstLineOffset()     const { return mFirstLineOffset; }
inline int ByteArrayTableLayout::noOfBytesPerLine()        const { return mNoOfBytesPerLine; }
inline Address ByteArrayTableLayout::byteArrayOffset()     const { return mByteArrayOffset; }
inline Size ByteArrayTableLayout::length()                 const { return mLastByteArrayOffset-mByteArrayOffset+1; }
inline Address ByteArrayTableLayout::lastByteArrayOffset() const { return mLastByteArrayOffset; }

inline Coord ByteArrayTableLayout::finalCoord()      const { return mCoordRange.end(); }
inline Coord ByteArrayTableLayout::startCoord()      const { return mCoordRange.start(); }
inline int ByteArrayTableLayout::startLinePosition() const { return startCoord().pos(); }
inline int ByteArrayTableLayout::finalLinePosition() const { return finalCoord().pos(); }
inline int ByteArrayTableLayout::startLine()         const { return startCoord().line(); }
inline int ByteArrayTableLayout::finalLine()         const { return finalCoord().line(); }
inline int ByteArrayTableLayout::noOfLinesPerPage()  const { return mNoOfLinesPerPage; }
inline int ByteArrayTableLayout::noOfLines()         const { return mByteArrayOffset>mLastByteArrayOffset?0:finalLine()+1; }

}

#endif
