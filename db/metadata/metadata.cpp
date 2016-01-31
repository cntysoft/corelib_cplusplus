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

const QStringList& Metadata::getSchemas()
{
   return m_source->getSchemas();
}

QStringList Metadata::getTableNames(QString schema, bool includeViews)
{
   QStringList tableNames;
   m_source->getTableNames(tableNames, schema, includeViews);
   return tableNames;
}

}//metadata
}//db
}//corelib
}//sn