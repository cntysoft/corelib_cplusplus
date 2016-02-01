#ifndef SN_CORELIB_DB_METADATA_METADATA_H
#define SN_CORELIB_DB_METADATA_METADATA_H

#include <QSharedPointer>

#include "global/global.h"
#include "kernel/errorinfo.h"

#include "db/engine/engine.h"
#include "db/metadata/source/abstract_source.h"
#include "db/metadata/object/abstract_table_object.h"
#include "db/metadata/object/column_object.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{

using sn::corelib::db::engine::Engine;
using sn::corelib::db::metadata::source::AbstractSource;
using sn::corelib::db::metadata::object::AbstractTableObject;
using sn::corelib::db::metadata::object::ColumnObject;

using sn::corelib::ErrorInfo;

class SN_CORELIB_EXPORT Metadata
{
public:
   Metadata(Engine *engine);
   Metadata(QSharedPointer<Engine> engine);
public:
   const QStringList& getSchemas();
   QStringList getTableNames(QString schema = QString(), bool includeViews = false);
   QList<QSharedPointer<AbstractTableObject>> getTables(QString schema = QString(), bool includeViews = false);
   QSharedPointer<AbstractTableObject> getTable(const QString &tableName, QString schema = QString())throw(ErrorInfo);
   QSharedPointer<ColumnObject> getColumn(const QString &columnName, const QString &tableName, 
                                          QString schema = QString());
protected:
   AbstractSource* createSourceFromEngine()throw(ErrorInfo);
protected:
   QSharedPointer<Engine> m_engine;
   QSharedPointer<AbstractSource> m_source;
};

}//metadata
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_METADATA_METADATA_H
