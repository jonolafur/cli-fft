/*
 * StreamHandler.cpp
 *
 *  Created on: Apr 14, 2013
 *      Author: jon
 */

#include "StreamHandler.h"

StreamHandler::StreamHandler() : m_in_stream( NULL ), m_in_file(), m_fileName(),
   m_commentChar( '#' ), m_lineCount( 0 ), m_readLineCount( 0 )
{}

StreamHandler::StreamHandler( const std::string& fileName, char commentChar ) :
   m_in_stream( NULL ), m_in_file(), m_fileName( fileName ),
   m_commentChar( commentChar ), m_lineCount( 0 ), m_readLineCount( 0 )
{
   init( fileName, commentChar );
}

StreamHandler::~StreamHandler()
{
   m_in_file.close();
   m_in_stream = NULL;
}

void StreamHandler::init( const std::string& fileName, char commentChar )
{
   m_commentChar = commentChar;

   // Check and re-direct the input source:
   if ( fileName.empty() )
   {
      m_in_stream = &std::cin;
   }
   else
   {
      m_in_file.open( fileName.c_str() );

      if ( !m_in_file.good() )
      {
         throw "Failed to open input file: " + fileName + "\n";
      }

      m_in_stream = &m_in_file;
   }
}

bool StreamHandler::getline( std::string& rawLine )
{
   bool ret = false;

   if ( std::getline( *m_in_stream, rawLine ) )
   {
      ret = true;
   }

   m_lineCount++;

   while ( isCommentedOut( rawLine ) && ret )
   {
      if ( std::getline( *m_in_stream, rawLine ) )
      {
         ret = true;
      }
      else
      {
         ret = false;
      }

      m_lineCount++;
   }

   m_readLineCount++;

   return ret;
}

std::string StreamHandler::filename()
{
   if ( m_fileName.empty() )
   {
      return "Standard input";
   }

   return m_fileName;
}

///////////////////////////////////////////////////////////////////////////////
// True if the first character in the line is equal to the comment character.
bool StreamHandler::isCommentedOut( const std::string& line )
{
   if ( line.empty() )
   {
      return true;
   }

   return ( line[0] == m_commentChar );
}



