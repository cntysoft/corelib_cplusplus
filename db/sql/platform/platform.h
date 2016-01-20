#ifndef SN_CORELIB_DB_SQL_PLATFORM_PLATFORM_H
#define SN_CORELIB_DB_SQL_PLATFORM_PLATFORM_H

#include "abstract_platform.h"
#include "global/global.h"
#include "db/engine/engine.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{

class SN_CORELIB_EXPORT Platform : public AbstractPlatform
{
public:
   Platform(Engine &engine);
protected:
   Engine& m_engine;
};


}//platform
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_PLATFORM_PLATFORM_H
