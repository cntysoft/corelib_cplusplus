#ifndef SN_CORELIB_DB_SQL_INTERFACE_H
#define SN_CORELIB_DB_SQL_INTERFACE_H

#include "global/global.h"
#include "db/engine/engine.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::db::engine::Engine;

class SN_CORELIB_EXPORT SqlInterface
{
public:
   QString getSqlString();
};

class SN_CORELIB_EXPORT PreparableSqlInterface
{
public:
   virtual void prepareStatement(const Engine &engine, );
};

class SN_CORELIB_EXPORT PlatformDecoratorInterface
{
public:
   virtual void setSubject() = 0;
};


}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_INTERFACE_H
