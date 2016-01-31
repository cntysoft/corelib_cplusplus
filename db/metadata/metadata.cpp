#include "metadata.h"
#include "db/metadata/source/mysql_metadata.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{

Metadata::Metadata(engine::Engine *engine)
   : Metadata(QSharedPointer<Engine>(engine))
{
   
}

Metadata::Metadata(QSharedPointer<Engine> engine)
   : m_engine(engine),
     m_source(createSourceFromEngine())
{
   
}

AbstractSource* Metadata::createSourceFromEngine()throw(ErrorInfo)
{
   Engine::PlatformType type = m_engine->getPlatformType();
   if(Engine::PlatformType::Mysql == type){
      return new source::MysqlMetadata(m_engine);
   }
   throw ErrorInfo("Metadata::createSourceFromEngine Platform type not supported");
}

const QVariant& Metadata::getSchemas()
{
   return m_source->getSchemas();
}

}//metadata
}//db
}//corelib
}//sn