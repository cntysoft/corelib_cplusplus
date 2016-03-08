#ifndef SN_CORELIB_DB_METADATA_OBJECT_COLUMN_OBJECT_H 
#define SN_CORELIB_DB_METADATA_OBJECT_COLUMN_OBJECT_H

#include <QString>
#include <QMap>
#include <QVariant>

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
   ColumnObject& setOrdinalPosition(int ordinalPosition);
   int getOrdinalPosition()const;
   const QString& getColumnDefault()const;
   ColumnObject& setColumnDefault(const QString &columnDefault);
   bool isNullable()const;
   ColumnObject& setIsNullable(bool flag);
   const QString& getDataType()const;
   ColumnObject& setDataType(const QString &dataType);
   int getCharacterMaximumLength()const;
   ColumnObject& setCharacterMaximumLength(int characterMaximumLength);
   int getCharacterOctetLength()const;
   ColumnObject& setCharacterOctetLength(int characterOctetLength);
   int getNumericPrecision()const;
   ColumnObject& setNumericPrecision(int numericPrecision);
   int getNumericScale()const;
   ColumnObject& setNumericScale(int numericScale);
   bool getNumericUnsigned()const;
   ColumnObject& setNumericUnsigned(bool numericUnsigned);
   int isNumericUnsigned()const;
   QMap<QString, QVariant> getErratas()const;
   ColumnObject& setErratas(const QMap<QString, QVariant> &erratas);
   QVariant getErrata(const QString &errataName);
   ColumnObject& setErrata(const QString &errataName, const QVariant &errataValue);
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
   QMap<QString, QVariant> m_errata;
};

}//object
}//metadata
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_METADATA_OBJECT_COLUMN_OBJECT_H

