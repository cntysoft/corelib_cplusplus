#include "simple_column.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace ddl{
namespace column {

AbstractExpression::ExpressionDataType Integer::getExpressionData()const
{
   ExpressionDataType data = AbstractColumn::getExpressionData();
   if(m_options.contains("length")){
      QList<QVariant> targetList = data[0].toList();
      QStringList params = targetList[1].value<QStringList>();
      targetList[1] = QVariant::fromValue(params);
      params[1] += "(" + m_options.value("length").toString() + ")";
      data[0] = QVariant::fromValue(targetList);
   }
   return data;
}

BigInteger::BigInteger()
{
   m_type = "BIGINT";
}

Binary::Binary(const QString &name, const QVariant & length, bool nullable,
               const QVariant &defaultValue, const QMap<QString, QVariant> &options)
   : AbstractLengthColumn(name, length, nullable, defaultValue, options)
{
   m_type = "BINARY";
}

Blob::Blob(const QString &name, const QVariant & length, bool nullable,
           const QVariant &defaultValue, const QMap<QString, QVariant> &options)
   : AbstractLengthColumn(name, length, nullable, defaultValue, options)
{
   m_type = "BLOB";
}

Char::Char(const QString &name, const QVariant & length, bool nullable,
           const QVariant &defaultValue, const QMap<QString, QVariant> &options)
   : AbstractLengthColumn(name, length, nullable, defaultValue, options)
{
   m_type = "CHAR";
}

Varchar::Varchar(const QString &name, const QVariant & length, bool nullable,
           const QVariant &defaultValue, const QMap<QString, QVariant> &options)
   : AbstractLengthColumn(name, length, nullable, defaultValue, options)
{
   m_type = "VARCHAR";
}

Text::Text(const QString &name, const QVariant & length, bool nullable,
           const QVariant &defaultValue, const QMap<QString, QVariant> &options)
   : AbstractLengthColumn(name, length, nullable, defaultValue, options)
{
   m_type = "TEXT";
}

Boolean::Boolean(const QString &name, bool nullable, const QVariant &defaultValue, 
                 const QMap<QString, QVariant> &options)
   : AbstractColumn(name, nullable, defaultValue, options)
{
   m_type = "BOOLEAN";
   m_isNullable = false;
}

Boolean& Boolean::setNullable()
{
   m_isNullable = false;
   return *this;
}

Date::Date(const QString &name, bool nullable, const QVariant &defaultValue, 
           const QMap<QString, QVariant> &options)
   : AbstractColumn(name, nullable, defaultValue, options)
{
   m_type = "DATE";
}

Datetime::Datetime(const QString &name, bool nullable, const QVariant &defaultValue, 
                   const QMap<QString, QVariant> &options)
   : AbstractColumn(name, nullable, defaultValue, options)
{
   m_type = "DATE";
}

Time::Time(const QString &name, bool nullable, const QVariant &defaultValue, 
                   const QMap<QString, QVariant> &options)
   : AbstractColumn(name, nullable, defaultValue, options)
{
   m_type = "TIME";
}

Timestamp::Timestamp(const QString &name, bool nullable, const QVariant &defaultValue, 
                     const QMap<QString, QVariant> &options)
   : AbstractTimestampColumn(name, nullable, defaultValue, options)
{
   
}

Decimal::Decimal(const QString &name, const QVariant &digits, 
                 const QVariant &decimal, bool nullable, const QVariant &defaultValue, 
                 const QMap<QString, QVariant> &options)
   :AbstractPrecisionColumn(name, digits, decimal, nullable, defaultValue, options)
{
   m_type = "DECIMAL";
}

Float::Float(const QString &name, const QVariant &digits, 
             const QVariant &decimal, bool nullable, const QVariant &defaultValue, 
             const QMap<QString, QVariant> &options)
   :AbstractPrecisionColumn(name, digits, decimal, nullable, defaultValue, options)
{
   m_type = "DECIMAL";
}

}//column
}//ddl
}//sql
}//db
}//corelib
}//sn