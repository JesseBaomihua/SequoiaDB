/*******************************************************************************


   Copyright (C) 2011-2014 SequoiaDB Ltd.

   This program is free software: you can redistribute it and/or modify
   it under the term of the GNU Affero General Public License, version 3,
   as published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warrenty of
   MARCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with this program. If not, see <http://www.gnu.org/license/>.

   Source File Name = pmdStartupHstLogger.hpp

   Descriptive Name = pmd start-up history logger

   Dependencies: N/A

   Restrictions: N/A

   Change Activity:
   defect Date        Who Description
   ====== =========== ======== ==============================================
          01/01/2018  Ting YU  Initial Draft

   Last Changed =

*******************************************************************************/

#ifndef PMD_STARTUPHSTLOGGER_HPP_
#define PMD_STARTUPHSTLOGGER_HPP_

#include <vector>
#include "ossFile.hpp"
#include "ossUtil.hpp"
#include "ossTypes.hpp"
#include "pmdStartup.hpp"

#define PMD_STARTUP_FILE_NAME          ".SEQUOIADB_STARTUP"

namespace engine
{
   /*
      _pmdStartupHstLogger define
      start-up history logger
   */
   #define PMD_STARTUP_LOG_LEN_MAX     ( 64 )
   #define PMD_STARTUP_LOG_HEADER      "pid, startTime, startType"OSS_NEWLINE
   #define PMD_STARTUP_LOG_HEADER_SIZE ( sizeof( PMD_STARTUP_LOG_HEADER ) - 1 )
   #define PMD_STARTUP_LOG_FIELD_NUM   ( 3 )
   #define PMD_STARTUP_FILESIZE_LIMIT  ( 1000 * 1024 )
   #define PMD_STARTUP_LOGSIZE_MAX     ( 100 * 1024 )

   struct _pmdStartupLog
   {
      OSSPID            _pid ;
      ossTimestamp      _time ;
      SDB_START_TYPE    _type ;

      _pmdStartupLog()
      :_pid( OSS_INVALID_PID ), _type( SDB_START_NORMAL )
      {
      }

      _pmdStartupLog( OSSPID pid, ossTimestamp time, SDB_START_TYPE type )
      :_pid( pid ), _time( time ), _type( type )
      {
      }

      string toString()
      {
         CHAR strTime[ OSS_TIMESTAMP_STRING_LEN + 1 ] = { 0 } ;
         CHAR strLog[ PMD_STARTUP_LOG_LEN_MAX ] = { 0 } ;

         ossTimestampToString( _time, strTime ) ;
         ossSnprintf( strLog, sizeof( strLog ) - 1, "%d,%s,%s"OSS_NEWLINE,
                      _pid, strTime, pmdGetStartTypeStr( _type ) ) ;
         return strLog ;
      }
   } ;
   typedef _pmdStartupLog pmdStartupLog ;

   BOOLEAN pmdStr2StartupLog( const string& str, pmdStartupLog& log ) ;

   class _pmdStartupHstLogger : public SDBObject
   {
      public:
         _pmdStartupHstLogger () ;
         ~_pmdStartupHstLogger () ;

         INT32 init() ;
         INT32 clearAll() ;
         INT32 getLatestLogs( UINT32 num, vector<pmdStartupLog> &vecLogs );

      protected:
         INT32 _buildFileName() ;
         INT32 _clearEarlyLogs() ;
         INT32 _loadLogs() ;
         INT32 _log() ;

      private:
         ossFile _file ;
         vector<pmdStartupLog> _buffer ;
         BOOLEAN _initOk ;
         CHAR _fileName[ OSS_MAX_PATHSIZE + 1 ] ;
   };

   typedef _pmdStartupHstLogger pmdStartupHstLogger ;

   pmdStartupHstLogger* pmdGetStartupHstLogger () ;

}

#endif //PMD_STARTUPHSTLOGGER_HPP_

