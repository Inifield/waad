/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentcommunity.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef WOWSERVER_SINGLETON_H
#define WOWSERVER_SINGLETON_H

#include "Errors.h"

/// Should be placed in the appropriate .cpp file somewhere
#define initialiseSingleton( type ) \
  template <> type * Singleton < type > :: mSingleton = NULL

/// To be used as a replacement for initialiseSingleton( )
///  Creates a file-scoped Singleton object, to be retrieved with getSingleton
#define createFileSingleton( type ) \
  initialiseSingleton( type ); \
  type the##type

template < class type > class SERVER_DECL Singleton {
public:
  /// Constructor
  Singleton( ) {
	/// If you hit this assert, this singleton already exists -- you can't create another one!
	//WPAssert( mSingleton == NULL );
   if (NULL == mSingleton)
   {
	mSingleton = static_cast<type *>(this);
   }
  }
  /// Destructor
  virtual ~Singleton( ) { mSingleton = NULL; } // Compatibilité Ascent

  static type &getSingleton() 
  {
   if (NULL == mSingleton)
   {
    /*mSingleton =*/ 
	   new type; // Full compatibilité Ascent
   }
   return (*mSingleton);
  }

  static void DestroySingleton ()
  {
   if (NULL != mSingleton)
   {
    delete mSingleton;
    /*mSingleton = NULL;*/ // Pas la peine, dans le destructeur (Brz)
   }
  }

  static type *getSingletonPtr( ) { return mSingleton; }

protected:
 
  /// Singleton pointer, must be set to 0 prior to creating the object
  static type * mSingleton;


};

#endif

