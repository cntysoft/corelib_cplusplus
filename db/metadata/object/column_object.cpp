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

ColumnObject& ColumnObject::setName(const QString &name)
{
   m_name = name;
   return *this;
}

const QString& ColumnObject::getName()const
{
   return m_name;
}
ColumnObject& ColumnObject::setTableName(const QString &tableName)
{
   m_tableName = tableName;
   return *this;
}

const QString& ColumnObject::getTableName()const
{
   return m_tableName;
}

ColumnObject& ColumnObject::setSchemaName(const QString &schemaName)
{
   m_schemaName = schemaName;
   return *this;
}

const QString& ColumnObject::getSchemaName()const
{
   return m_schemaName;
}

ColumnObject& ColumnObject::setOrdinalPosition(int ordinalPosition)
{
   m_ordinalPosition = ordinalPosition;
   return *this;
}

int ColumnObject::getOrdinalPosition()const
{
   return m_ordinalPosition;
}

const QString& ColumnObject::getColumnDefault()const
{
   return m_columnDefault;
}

ColumnObject& ColumnObject::setColumnDefault(const QString &columnDefault)
{
   m_columnDefault = columnDefault;
   return *this;
}

bool ColumnObject::isNullable()const
{
   return m_isNullable;
}

ColumnObject& ColumnObject::setIsNullable(bool flag)
{
   m_isNullable = flag;
   return *this;
}

const QString& ColumnObject::getDataType()const
{
   return m_dataType;
}

ColumnObject& ColumnObject::setDataType(const QString &dataType)
{
   m_dataType = dataType;
   return *this;
}

int ColumnObject::getCharacterMaximumLength()const
{
   return m_characterMaximumLength;
}

ColumnObject& ColumnObject::setCharacterMaximumLength(int characterMaximumLength)
{
   m_characterMaximumLength = characterMaximumLength;
   return *this;
}

int ColumnObject::getCharacterOctetLength()const
{
   return m_characterOctetLength;
}

ColumnObject& ColumnObject::setCharacterOctetLength(int characterOctetLength)
{
   m_characterOctetLength = characterOctetLength;
   return *this;
}

int ColumnObject::getNumericPrecision()const
{
   return m_numericPrecision;
}

ColumnObject& ColumnObject::setNumericPrecision(int numericPrecision)
{
   m_numericPrecision = numericPrecision;
   return *this;
}

int ColumnObject::getNumericScale()const
{
   return m_numericScale;
}

ColumnObject& ColumnObject::setNumericScale(int numericScale)
{
   m_numericScale = numericScale;
   return *this;
}

bool ColumnObject::getNumericUnsigned()const
{
   return m_numericUnsigned;
}

ColumnObject& ColumnObject::setNumericUnsigned(bool numericUnsigned)
{
   m_numericUnsigned = numericUnsigned;
   return *this;
}

int ColumnObject::isNumericUnsigned()const
{
   return m_numericUnsigned;
}

QMap<QString, QVariant> ColumnObject::getErratas()const
{
   return m_errata;
}


ColumnObject& ColumnObject::setErratas(const QMap<QString, QVariant> &erratas)
{
   QMap<QString, QVariant>::const_iterator it = erratas.cbegin();
   QMap<QString, QVariant>::const_iterator endMarker = erratas.cend();
   while(it != endMarker){
      m_errata.insert(it.key(), it.value());
      it++;
   }
   return *this;
}

QVariant ColumnObject::getErrata(const QString &errataName)
{
   if(m_errata.contains(errataName)){
      return m_errata.value(errataName);
   }
   return QString();
}

ColumnObject& ColumnObject::setErrata(const QString &errataName, const QVariant &errataValue)
{
   m_errata.insert(errataName, errataValue);
   return *this;
}

}//object
}//metadata
}//db
}//corelib
}//sn