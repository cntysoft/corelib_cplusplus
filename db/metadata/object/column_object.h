#ifndef SN_CORELIB_DB_METADATA_OBJECT_COLUMN_OBJECT_H 
#define SN_CORELIB_DB_METADATA_OBJECT_COLUMN_OBJECT_H

#include <QString>

#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace object{

class SN_CORELIB_EXPORT ColumnObject
{
public:
   ColumnObject(const QString &name, const QString &tableName, const QString &schemaName = QString());
   ColumnObject& setName(const QString &name);
   const QString& getName()const;
   ColumnObject& setTableName(const QString &tableName);
   const QString& getTableName()const;
   ColumnObject& setSchemaName(const QString &schemaName);
   const QString& getSchemaName()const;
protected:
   QString m_name;
   QString m_tableName;
   QString m_schemaName;
   int m_ordinalPosition;
   QString m_columnDefault;
   bool m_isNullable;
   QString m_dataType;
   int m_characterMaximumLength;
   int m_characterOctetLength;
   int m_numericPrecision;
   int m_numericScale;
   bool m_numericUnsigned;
   QMap<QString, QString> m_errata;
};

}//object
}//metadata
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_METADATA_OBJECT_COLUMN_OBJECT_H

