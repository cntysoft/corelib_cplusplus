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

Char::Char(const QString &name, const QVariant & length, bool nullable,
           const QVariant &defaultValue, const QMap<QString, QVariant> &options)
   : AbstractLengthColumn(name, length, nullable, defaultValue, options)
{
   m_type = "CHAR";
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



}//column
}//ddl
}//sql
}//db
}//corelib
}//sn