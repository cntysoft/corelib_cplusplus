#ifndef SN_CORELIB_DB_METADATA_SOURCE_ABSTRACT_SOURCE_H
#define SN_CORELIB_DB_METADATA_SOURCE_ABSTRACT_SOURCE_H

#include <QSharedPointer>
#include <QMap>
#include <QVariant>

#include "global/global.h"
#include "db/engine/engine.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace source{

using sn::corelib::db::engine::Engine;

class SN_CORELIB_EXPORT AbstractSource
{
   const static QString __DEFAULT_SCHEMA__;
public:
   AbstractSource(QSharedPointer<Engine> engine);
   const QVariant& getSchemas();
public:
   virtual ~AbstractSource();
protected:
   virtual void loadSchemaData() = 0;
protected:
   QSharedPointer<Engine> m_engine;
   QString m_defaultSchema;
   QMap<QString, QVariant> m_data;
};


}//source
}//metadata
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_METADATA_SOURCE_ABSTRACT_SOURCE_H
