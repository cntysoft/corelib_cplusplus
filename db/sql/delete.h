#ifndef SN_CORELIB_DB_SQL_DELETE_H
#define SN_CORELIB_DB_SQL_DELETE_H

#include <QString>

#include "global/global.h"
#include "abstract_preparable_sql.h"
#include "table_identifier.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

class SN_CORELIB_EXPORT Delete : public AbstractPreparableSql
{
   Q_OBJECT
public:
   const static QString SPECIFICATION_DELETE;
   const static QString SPECIFICATION_WHERE;
public:
   Delete(const TableIdentifier &table);
   Delete(const QString &table);
   TableIdentifier& getTable();
   ~Delete();
protected:
   TableIdentifier m_table;
   bool m_emptyWhereProtection = true;
};

}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_DELETE_H
