#ifndef SN_CORELIB_DB_ENGINE_ENGINE_H
#define SN_CORELIB_DB_ENGINE_ENGINE_H

#include <QString>
#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{
namespace engine{

class SN_CORELIB_EXPORT Engine
{
public:
   const static QString QUERY_MODE_EXECUTE;
   const static QString QUERY_MODE_PREPARE;
   const static QString PREPARE_TYPE_POSITIONAL;
   const static QString PREPARE_TYPE_NAMED;
};

}//engine
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_ENGINE_ENGINE_H

