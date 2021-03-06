#ifndef SN_CORELIB_DB_SQL_DDL_COLUMN_SIMPLE_COLUMN_H
#define SN_CORELIB_DB_SQL_DDL_COLUMN_SIMPLE_COLUMN_H

#include "abstract_column.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace ddl{
namespace column {

class SN_CORELIB_EXPORT Integer : public AbstractColumn
{
public:
   Integer(const QString &name = QString(), bool nullable = false,
                  const QVariant &defaultValue = QVariant(), const QMap<QString, QVariant> &options = QMap<QString, QVariant>());
   virtual ExpressionDataType getExpressionData() const;
};

class SN_CORELIB_EXPORT BigInteger : Integer
{
public:
   BigInteger();
};

class SN_CORELIB_EXPORT Binary : public AbstractLengthColumn
{
public:
   Binary(const QString &name, const QVariant & length = QVariant(), bool nullable = false,
          const QVariant &defaultValue = QVariant(), const QMap<QString, QVariant> &options = QMap<QString, QVariant>());
};

class SN_CORELIB_EXPORT Blob : public AbstractLengthColumn
{
public:
   Blob(const QString &name, const QVariant & length = QVariant(), bool nullable = false,
        const QVariant &defaultValue = QVariant(), const QMap<QString, QVariant> &options = QMap<QString, QVariant>());
};

class SN_CORELIB_EXPORT Char : public AbstractLengthColumn
{
public:
   Char(const QString &name, const QVariant &length = QVariant(), bool nullable = false,
        const QVariant &defaultValue = QVariant(), const QMap<QString, QVariant> &options = QMap<QString, QVariant>());
};

class SN_CORELIB_EXPORT Varchar : public AbstractLengthColumn
{
public:
   Varchar(const QString &name, const QVariant &length = QVariant(), bool nullable = false,
           const QVariant &defaultValue = QVariant(), const QMap<QString, QVariant> &options = QMap<QString, QVariant>());
};

class SN_CORELIB_EXPORT Text : public AbstractLengthColumn
{
public:
   Text(const QString &name, const QVariant &length = QVariant(), bool nullable = false,
        const QVariant &defaultValue = QVariant(), const QMap<QString, QVariant> &options = QMap<QString, QVariant>());
};

class SN_CORELIB_EXPORT Varbinary : public AbstractLengthColumn
{
public:
   Varbinary(const QString &name, const QVariant &length = QVariant(), 
             bool nullable = false, const QVariant &defaultValue = QVariant(), 
             const QMap<QString, QVariant> &options = QMap<QString, QVariant>());
};

class SN_CORELIB_EXPORT Boolean : public AbstractColumn
{
public:
   Boolean(const QString &name = QString(), bool nullable = false,
           const QVariant &defaultValue = QVariant(), const QMap<QString, QVariant> &options = QMap<QString, QVariant>());
   Boolean& setNullable();
};

class SN_CORELIB_EXPORT Date : public AbstractColumn
{
public:
   Date(const QString &name = QString(), bool nullable = false,
        const QVariant &defaultValue = QVariant(), const QMap<QString, QVariant> &options = QMap<QString, QVariant>());
};

class SN_CORELIB_EXPORT Datetime : public AbstractColumn
{
public:
   Datetime(const QString &name = QString(), bool nullable = false,
            const QVariant &defaultValue = QVariant(), const QMap<QString, QVariant> &options = QMap<QString, QVariant>());
};

class SN_CORELIB_EXPORT Time : public AbstractColumn
{
public:
   Time(const QString &name = QString(), bool nullable = false,
        const QVariant &defaultValue = QVariant(), const QMap<QString, QVariant> &options = QMap<QString, QVariant>());
};


class SN_CORELIB_EXPORT Timestamp : public AbstractTimestampColumn
{
public:
   Timestamp(const QString &name = QString(), bool nullable = false,
             const QVariant &defaultValue = QVariant(), const QMap<QString, QVariant> &options = QMap<QString, QVariant>());
};

class SN_CORELIB_EXPORT Decimal : public AbstractPrecisionColumn
{
public:
   Decimal(const QString &name, const QVariant &digits = QVariant(), const QVariant &decimal = QVariant(), 
           bool nullable = false, const QVariant &defaultValue = QVariant(), 
           const QMap<QString, QVariant> &options = QMap<QString, QVariant>());
};

class SN_CORELIB_EXPORT Float : public AbstractPrecisionColumn
{
public:
   Float(const QString &name, const QVariant &digits = QVariant(), const QVariant &decimal = QVariant(), 
         bool nullable = false, const QVariant &defaultValue = QVariant(), 
         const QMap<QString, QVariant> &options = QMap<QString, QVariant>());
};



}//column
}//ddl
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_DDL_COLUMN_SIMPLE_COLUMN_H

