#include <QString>
#include <QSharedPointer>

#include "sql.h"
#include <typeinfo>


namespace sn{
namespace corelib{
namespace db{
namespace sql{


Sql::Sql(Engine &engine, const QString &table)
   : m_engine(engine),
     m_platfrom(engine)
{
   if(!table.isNull()){
      m_table.setTable(table);
   }
}

Sql::Sql(engine::Engine &engine, const TableIdentifier &table)
   : m_engine(engine),
     m_table(table),
     m_platfrom(engine)
{   
}

bool Sql::hasTable()
{
   return !m_table.getTable().isNull();
}

const QString& Sql::getTableName()
{
   return m_table.getTable();   
}

const TableIdentifier& Sql::getTable()
{
   return m_table;
}

Sql& Sql::setTable(const QString &table)
{
   m_table.setTable(table);
   return *this;
}

Sql& Sql::setTable(const TableIdentifier &table)
{
   m_table = table;
   return *this;
}

QSharedPointer<Delete> Sql::getDeleteSql(const QString &table)
{
   if(!table.isEmpty()){
      TableIdentifier targetTable(table);
      return QSharedPointer<Delete>(new Delete(targetTable));
   }
   return QSharedPointer<Delete>(new Delete(m_table));
}

QSharedPointer<Select> Sql::getSelectSql(const QString &table)
{
   if(!table.isEmpty()){
      TableIdentifier targetTable(table);
      return QSharedPointer<Select>(new Select(targetTable));
   }
   return QSharedPointer<Select>(new Select(m_table));
}

QSharedPointer<Insert> Sql::getInsertSql(const QString &table)
{
   if(!table.isEmpty()){
      TableIdentifier targetTable(table);
      return QSharedPointer<Insert>(new Insert(targetTable));
   }
   return QSharedPointer<Insert>(new Insert(m_table));
}

QSharedPointer<Update> Sql::getUpdateSql(const QString &table)
{
   if(!table.isEmpty()){
      TableIdentifier targetTable(table);
      return QSharedPointer<Update>(new Update(targetTable));
   }
   return QSharedPointer<Update>(new Update(m_table));
}

QSharedPointer<CreateTable> Sql::getCreateTableSql(const QString &table)
{
   if(!table.isEmpty()){
      TableIdentifier targetTable(table);
      return QSharedPointer<CreateTable>(new CreateTable(targetTable));
   }
   return QSharedPointer<CreateTable>(new CreateTable(m_table));
}

QSharedPointer<DropTable> Sql::getDropTableSql(const QString &table)
{
   if(!table.isEmpty()){
      TableIdentifier targetTable(table);
      return QSharedPointer<DropTable>(new DropTable(targetTable));
   }
   return QSharedPointer<DropTable>(new DropTable(m_table));
}


QString Sql::buildSqlString(QSharedPointer<AbstractSql> sqlObject)
{
   return m_platfrom.setSubject(sqlObject).getSqlString(m_engine);
}

}//sql
}//db
}//corelib
}//sn