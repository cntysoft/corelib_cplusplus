#include "column_object.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace object{

ColumnObject::ColumnObject(const QString &name, const QString &tableName, const QString &schemaName)
   : m_name(name),
     m_tableName(tableName),
     m_schemaName(schemaName)
{
   
}

}//object
}//metadata
}//db
}//corelib
}//sn