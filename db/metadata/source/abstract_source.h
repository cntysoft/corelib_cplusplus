#ifndef SN_CORELIB_DB_METADATA_SOURCE_ABSTRACT_SOURCE_H
#define SN_CORELIB_DB_METADATA_SOURCE_ABSTRACT_SOURCE_H

#include <QSharedPointer>
#include <QMap>
#include <QVariant>
#include <QList>

#include "global/global.h"
#include "db/engine/engine.h"
#include "db/metadata/object/abstract_table_object.h"
#include "db/metadata/object/column_object.h"
#include "kernel/errorinfo.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace source{

using sn::corelib::ErrorInfo;
using sn::corelib::db::engine::Engine;
using sn::corelib::db::metadata::object::AbstractTableObject;
using sn::corelib::db::metadata::object::ColumnObject;

class SN_CORELIB_EXPORT AbstractSource
{
public:
   const static QString __DEFAULT_SCHEMA__;
public:
   AbstractSource(QSharedPointer<Engine> engine);
   const QStringList& getSchemas();
   void getTableNames(QStringList &tableNames, QString schema = QString(), bool includeViews = false);
   void getTables(QList<QSharedPointer<AbstractTableObject>> &tables, QString schema = QString(), 
                  bool includeViews = false);
   QSharedPointer<AbstractTableObject> getTable(const QString &tableName, QString schema = QString())throw(ErrorInfo);
   QSharedPointer<ColumnObject> getColumn(const QString &columnName, const QString &tableName, 
                                                 QString schema = QString());
public:
   virtual ~AbstractSource();
protected:
   virtual void loadSchemaData() = 0;
   virtual void loadTableNameData(const QString &schema) = 0;
   virtual void loadColumnData(const QString &table, const QString &schema) = 0;
protected:
   QSharedPointer<Engine> m_engine;
   QString m_defaultSchema;
   QStringList m_schemasData;
   QMap<QString, QMap<QString, QMap<QString, QString>>> m_schemasTablesData;
   QMap<QString, QMap<QString, QMap<QString, QMap<QString, QVariant>>>> m_tableColumnsData;
};


}//source
}//metadata
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_METADATA_SOURCE_ABSTRACT_SOURCE_H
