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
public:
   const static QString __DEFAULT_SCHEMA__;
public:
   AbstractSource(QSharedPointer<Engine> engine);
   const QStringList& getSchemas();
   void getTableNames(QStringList &tableNames, QString schema = QString(), bool includeViews = false);
public:
   virtual ~AbstractSource();
protected:
   virtual void loadSchemaData() = 0;
   virtual void loadTableNameData(const QString &schema) = 0;
protected:
   QSharedPointer<Engine> m_engine;
   QString m_defaultSchema;
   QStringList m_schemasData;
   QMap<QString, QMap<QString, QMap<QString, QString>>> m_schemasTablesData;
};


}//source
}//metadata
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_METADATA_SOURCE_ABSTRACT_SOURCE_H
