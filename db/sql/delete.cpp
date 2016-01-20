#include "delete.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

const QString Delete::SPECIFICATION_WHERE = "where";
const QString Delete::SPECIFICATION_DELETE = "delete";

Delete::Delete(const QString &table)
   : Delete(TableIdentifier(table))
{
}

Delete::Delete(const TableIdentifier &table)
   :m_table(table)
{
//   m_specifications.insert(Delete::SPECIFICATION_DELETE, "DELETE FROM %1$s");
//   m_specifications.insert(Delete::SPECIFICATION_WHERE, "WHERE %1$s");
}

TableIdentifier& Delete::getTable()
{
   return m_table;
}

Delete::~Delete()
{}

}//sql
}//db
}//corelib
}//sn