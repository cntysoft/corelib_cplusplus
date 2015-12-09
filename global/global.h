#ifndef SN_CORELIB_GLOBAL_H
#define SN_CORELIB_GLOBAL_H

#include <qglobal.h>
#ifdef SN_CORELIB_STATIC_LIB
   #define SN_CORELIB_EXPORT 
#else
   #ifdef RMGR_LIBRARY
      #define SN_CORELIB_EXPORT Q_DECL_EXPORT
   #else
      #define SN_CORELIB_EXPORT Q_DECL_IMPORT
   #endif
#endif
#include "const.h"
#endif // SN_CORELIB_GLOBAL_H