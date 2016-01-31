#ifndef SN_CORELIB_DB_METADATA_SOURCE_MYSQL_METADATA_H
#define SN_CORELIB_DB_METADATA_SOURCE_MYSQL_METADATA_H

#include <QSharedPointer>

#include "db/engine/engine.h"
#include "global/global.h"
#include "abstract_source.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace source{

class SN_CORELIB_EXPORT MysqlMetadata : public AbstractSource
{
public:
   MysqlMetadata(QSharedPointer<Engine> engine);
public:
   virtual void loadSchemaData();
};

}//source
}//metadata
}//db
}//corelib
}//sn


#endif // SN_CORELIB_DB_METADATA_SOURCE_MYSQL_METADATA_H
