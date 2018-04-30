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

   Source File Name = monDMS.hpp

   Descriptive Name = Monitor Data Management Service Header

   When/how to use: this program may be used on binary and text-formatted
   versions of monitoring component. This file contains structure for
   DMS information.

   Dependencies: N/A

   Restrictions: N/A

   Change Activity:
   defect Date        Who Description
   ====== =========== === ==============================================
          09/14/2012  TW  Initial Draft

   Last Changed =

*******************************************************************************/
#ifndef MONDMS_HPP_
#define MONDMS_HPP_

#include "core.hpp"
#include "oss.hpp"
#include "dms.hpp"
#include "ossUtil.hpp"
#include "../bson/bson.h"
#include "../bson/bsonobj.h"
#include <set>
#include <vector>
using namespace std ;
using namespace bson ;

namespace engine
{
   /*
      _detailedInfo define
   */
   class _detailedInfo : public SDBObject
   {
   public :
      UINT32 _numIndexes ;
      UINT16 _blockID ;
      UINT16 _flag ;
      UINT32 _logicID ;

      UINT32 _attribute ;
      UINT8  _compressType ;
      UINT32 _hasDict ;

      UINT32 _pageSize ;
      UINT32 _lobPageSize ;

      // stat info
      UINT64 _totalRecords ;
      UINT64 _totalLobs ;
      UINT32 _totalDataPages ;
      UINT32 _totalIndexPages ;
      UINT32 _totalLobPages ;
      UINT64 _totalDataFreeSpace ;
      UINT64 _totalIndexFreeSpace ;
      // end

      _detailedInfo ()
      {
         _numIndexes          = 0 ;
         _blockID             = 0 ;
         _flag                = 0 ;
         _flag                = 0 ;
         _logicID             = 0 ;

         _attribute           = 0 ;
         _compressType        = 0 ;
         _hasDict             = FALSE ;

         _pageSize            = 0 ;
         _lobPageSize         = 0 ;

         _totalRecords        = 0 ;
         _totalLobs           = 0 ;
         _totalDataPages      = 0 ;
         _totalIndexPages     = 0 ;
         _totalLobPages       = 0 ;
         _totalDataFreeSpace  = 0 ;
         _totalIndexFreeSpace = 0 ;
      }
   } ;
   typedef class _detailedInfo detailedInfo ;

   /*
      _monCLSimple define
   */
   class _monCLSimple : public SDBObject
   {
      public:
         CHAR  _name[ DMS_COLLECTION_FULL_NAME_SZ + 1 ] ;

         _monCLSimple()
         {
            _name[ 0 ] = 0 ;
         }

         BOOLEAN operator<(const _monCLSimple &r) const
         {
            return ossStrncmp( _name, r._name, sizeof(_name))<0 ;
         }
   } ;
   typedef _monCLSimple monCLSimple ;

   /*
      _monCollection define
   */
   class _monCollection : public SDBObject
   {
   public :
      CHAR _name [ DMS_COLLECTION_FULL_NAME_SZ + 1 ] ;
      std::map<UINT32, detailedInfo>   _details ;

      _monCollection()
      {
         _name[ 0 ]  = 0 ;
      }
      OSS_INLINE BOOLEAN operator<(const _monCollection &r) const
      {
         return ossStrncmp( _name, r._name, sizeof(_name))<0 ;
      }
      OSS_INLINE detailedInfo& addDetails ( UINT32 sequence, UINT32 numIndexes,
                                            UINT16 blockID, UINT16 flag,
                                            UINT32 logicID, UINT64 totalRecords,
                                            UINT32 totalDataPages,
                                            UINT32 totalIndexPages,
                                            UINT32 totalLobPages,
                                            UINT64 totalDataFreeSpace,
                                            UINT64 totalIndexFreeSpace )
      {
         detailedInfo &info = _details[ sequence ] ;
         info._numIndexes = numIndexes ;
         info._blockID = blockID ;
         info._flag = flag ;
         info._logicID = logicID ;

         info._totalRecords        = totalRecords ;
         info._totalDataPages      = totalDataPages ;
         info._totalIndexPages     = totalIndexPages ;
         info._totalLobPages       = totalLobPages ;
         info._totalDataFreeSpace  = totalDataFreeSpace ;
         info._totalIndexFreeSpace = totalIndexFreeSpace ;

         return info ;
      }

   } ;
   typedef class _monCollection monCollection ;

   /*
      _monCSSimple define
   */
   class _monCSSimple : public SDBObject
   {
      public:
         CHAR  _name[ DMS_COLLECTION_SPACE_NAME_SZ + 1 + 1 ] ;

         _monCSSimple()
         {
            _name[ 0 ] = 0 ;
         }

         BOOLEAN operator<(const _monCSSimple &r) const
         {
            return ossStrncmp( _name, r._name, sizeof(_name))<0 ;
         }
   } ;
   typedef _monCSSimple monCSSimple ;

   /*
      _monCollectionSpace define
   */
   class _monCollectionSpace : public SDBObject
   {
   public :
      CHAR _name [ DMS_COLLECTION_SPACE_NAME_SZ + 1 ] ;
      vector<monCLSimple> _collections ;
      INT32 _pageSize ;
      INT32 _clNum ;
      INT64 _totalRecordNum ;
      INT64 _totalSize ;
      INT64 _freeSize ;
      INT32 _lobPageSize ;
      INT64 _totalDataSize ;
      INT64 _totalIndexSize ;
      INT64 _totalLobSize ;
      INT64 _freeDataSize ;
      INT64 _freeIndexSize ;
      INT64 _freeLobSize ;
      UINT64 _dataLsn ;
      UINT64 _lobLsn ;
      UINT8  _committed ;
      string _committedDesc ;

      _monCollectionSpace ()
      {
         ossMemset ( _name, 0, sizeof(_name)) ;
         _pageSize = 0 ;
         _clNum    = 0 ;
         _totalRecordNum = 0 ;
         _totalSize = 0 ;
         _freeSize  = 0 ;
         _lobPageSize = 0 ;
         _totalDataSize = 0 ;
         _totalIndexSize = 0 ;
         _totalLobSize = 0 ;
         _freeDataSize = 0 ;
         _freeIndexSize = 0 ;
         _freeLobSize = 0 ;
         _dataLsn = -1 ;
         _lobLsn = -1 ;
         _committed = 0 ;
      }
      _monCollectionSpace ( const _monCollectionSpace &right )
      {
         ossMemcpy ( _name, right._name, sizeof(_name) ) ;
         _collections = right._collections ;
         _pageSize = right._pageSize ;
         _clNum    = right._clNum ;
         _totalRecordNum = right._totalRecordNum ;
         _totalSize = right._totalSize ;
         _freeSize  = right._freeSize ;
         _lobPageSize = right._lobPageSize ;
         _totalDataSize = right._totalDataSize ;
         _totalIndexSize = right._totalIndexSize ;
         _totalLobSize = right._totalLobSize ;
         _freeDataSize = right._freeDataSize ;
         _freeIndexSize = right._freeIndexSize ;
         _freeLobSize = right._freeLobSize ;
         _dataLsn = right._dataLsn ;
         _lobLsn = right._lobLsn ;
         _committed = right._committed ;
         _committedDesc = right._committedDesc ;
      }
      ~_monCollectionSpace()
      {
         _collections.clear() ;
      }

      OSS_INLINE BOOLEAN operator<(const _monCollectionSpace &r) const
      {
         return ossStrncmp( _name, r._name, sizeof(_name))<0 ;
      }
      _monCollectionSpace &operator= (const _monCollectionSpace &right)
      {
         ossMemcpy ( _name, right._name, sizeof(_name) ) ;
         _collections = right._collections ;
         _pageSize = right._pageSize ;
         _clNum    = right._clNum ;
         _totalRecordNum = right._totalRecordNum ;
         _totalSize      = right._totalSize ;
         _freeSize       = right._freeSize ;
         _lobPageSize    = right._lobPageSize ;
         _totalDataSize = right._totalDataSize ;
         _totalIndexSize = right._totalIndexSize ;
         _totalLobSize = right._totalLobSize ;
         _freeDataSize = right._freeDataSize ;
         _freeIndexSize = right._freeIndexSize ;
         _freeLobSize = right._freeLobSize ;
         _dataLsn = right._dataLsn ;
         _lobLsn = right._lobLsn ;
         _committed = right._committed ;
         _committedDesc = right._committedDesc ;

         return *this ;
      }
   } ;
   typedef class _monCollectionSpace monCollectionSpace ;

   /*
      _monStorageUnit define
   */
   class _monStorageUnit : public SDBObject
   {
   public :
      CHAR _name [ DMS_COLLECTION_SPACE_NAME_SZ + 1 ] ;
      dmsStorageUnitID _CSID ;
      UINT32 _logicalCSID ;
      SINT32 _pageSize ;
      SINT32 _lobPageSize ;
      SINT32 _sequence ;
      SINT32 _numCollections ;
      SINT32 _collectionHWM ;
      SINT64 _size ;

      OSS_INLINE BOOLEAN operator<(const _monStorageUnit &r) const
      {
         SINT32 rc = ossStrncmp( _name, r._name, sizeof(_name))<0 ;
         // if two storage unit got same name, let's check sequence
         if ( !rc )
            return _sequence < r._sequence ;
         return rc ;
      }

      _monStorageUnit()
      {
         _name[ 0 ] = 0 ;
         _CSID = -1 ;
         _logicalCSID = 0 ;
         _pageSize = 0 ;
         _lobPageSize = 0 ;
         _sequence = 0 ;
         _numCollections = 0 ;
         _collectionHWM = 0 ;
         _size = 0 ;
      }
   } ;
   typedef class _monStorageUnit monStorageUnit ;

   /*
      _monIndex define
   */
   class _monIndex : public SDBObject
   {
   public:
      UINT16         _indexFlag ;
      CHAR           _version ;
      dmsExtentID    _scanExtLID ;
      BSONObj        _indexDef ;

      _monIndex()
      {
         _indexFlag = 0 ;
         _version = 0 ;
         _scanExtLID = -1 ;
      }
   } ;
   typedef _monIndex monIndex ;

}

#endif //MONDMS_HPP_
