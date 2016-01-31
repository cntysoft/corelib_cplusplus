#include "mysql_metadata.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace source{

MysqlMetadata::MysqlMetadata(QSharedPointer<engine::Engine> engine)
   : AbstractSource(engine)
{
   
}

void MysqlMetadata::loadSchemaData()
{
   
}

}//source
}//metadata
}//db
}//corelib
}//sn