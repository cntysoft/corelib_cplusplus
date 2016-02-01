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

QList<QSharedPointer<AbstractTableObject>> Metadata::getTables(QString schema, bool includeViews)
{
   QList<QSharedPointer<AbstractTableObject>> tables;
   m_source->getTables(tables, schema, includeViews);
   return tables;
}

QSharedPointer<AbstractTableObject> Metadata::getTable(const QString &tableName, QString schema)throw(ErrorInfo)
{
   return m_source->getTable(tableName, schema);
}

QStringList Metadata::getViewNames(QString schema)
{
   QStringList viewNames;
   m_source->getViewNames(viewNames, schema);
   return viewNames;
}

QList<QSharedPointer<ViewObject>> Metadata::getViews(QString schema)
{
   QList<QSharedPointer<ViewObject>> views;
   m_source->getViews(views, schema);
   return views;
}

QSharedPointer<ViewObject> Metadata::getView(const QString &viewName, QString schema)
{
   return m_source->getView(viewName, schema);
}

QSharedPointer<ColumnObject> Metadata::getColumn(const QString &columnName, const QString &tableName, 
                                                 QString schema)
{
   return m_source->getColumn(columnName, tableName, schema);
}

QList<QSharedPointer<ColumnObject>> Metadata::getColumns(const QString &tableName, QString schema)
{
   QList<QSharedPointer<ColumnObject>> columns;
   m_source->getColumns(columns, tableName, schema);
   return columns;
}

QStringList Metadata::getColumnNames(const QString &tableName, QString schema)
{
   QStringList names;
   m_source->getColumnNames(names, tableName, schema);
   return names;
}

QSharedPointer<ConstraintObject> Metadata::getConstraint(const QString &constraintName, const QString &table, 
                                               QString schema)
{
   return m_source->getConstraint(constraintName, table, schema);
}

QList<QSharedPointer<ConstraintObject>> Metadata::getConstraints(const QString &table, QString schema)
{
   QList<QSharedPointer<ConstraintObject>> constraints;
   m_source->getConstraints(constraints, table, schema);
   return constraints;
}

}//metadata
}//db
}//corelib
}//sn