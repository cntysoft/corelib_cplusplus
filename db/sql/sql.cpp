#include <QString>

#include "sql.h"
#include "interface.h"

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

}//sql
}//db
}//corelib
}//sn