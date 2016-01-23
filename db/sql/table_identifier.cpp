#include "table_identifier.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

TableIdentifier::TableIdentifier(const QString &table, const QString &schema)
   :m_table(table)
{
   if(!schema.isNull()){
      m_schema = schema;
   }
}

const QString& TableIdentifier::getTable()const
{
   return m_table;
}

TableIdentifier& TableIdentifier::setTable(const QString &table)
{
   m_table = table;
   return *this;
}

bool TableIdentifier::hasSchema()
{
   return m_schema.isNull();
}

TableIdentifier& TableIdentifier::setSchema(const QString &schema)
{
   m_schema = schema;
   return *this;
}

const QString& TableIdentifier::getSchema()const
{
   return m_schema;
}

QPair<QString, QString> TableIdentifier::getTableAndSchema()const
{
   return QPair<QString, QString>{
      m_schema, m_table
   };
}

bool TableIdentifier::isNull()const
{
   return m_table.isNull() || m_table.isEmpty();
}

}//sql
}//db
}//corelib
}//sn