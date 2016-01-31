#include "abstract_source.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace source{

const QString AbstractSource::__DEFAULT_SCHEMA__ = "__DEFAULT_SCHEMA__";

AbstractSource::AbstractSource(QSharedPointer<engine::Engine> engine)
   : m_engine(engine)
{
   QString currentSchema(m_engine->getCurrentSchema());
   if(!currentSchema.isEmpty()){
      m_defaultSchema = currentSchema;
   }else{
      m_defaultSchema = AbstractSource::__DEFAULT_SCHEMA__;
   }
}

const QVariant& AbstractSource::getSchemas()
{
   loadSchemaData();
   return m_data["schemas"];
}


AbstractSource::~AbstractSource()
{}

}//source
}//metadata
}//db
}//corelib
}//sn