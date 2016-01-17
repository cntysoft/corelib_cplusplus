#ifndef SN_CORELIB_DB_SQL_TABLE_IDENTIFIER_H
#define SN_CORELIB_DB_SQL_TABLE_IDENTIFIER_H

#include <QString>
#include <QPair>

#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

class SN_CORELIB_EXPORT TableIdentifier
{
public:
   TableIdentifier(const QString &table, const QString &schema  = QString());
   TableIdentifier& setTable(const QString &table);
   const QString& getTable();
   bool hasSchema();
   TableIdentifier& setSchema(const QString &schema);
   const QString& getSchema();
   QPair<QString, QString> getTableAndSchema();
protected:
   QString m_table;
   QString m_schema;
};

}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_TABLE_IDENTIFIER_H