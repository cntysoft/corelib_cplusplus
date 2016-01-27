#ifndef SN_CORELIB_DB_SQL_SIMPLE_SQL_H
#define SN_CORELIB_DB_SQL_SIMPLE_SQL_H

#include "global/global.h"
#include "predicate/predicate.h"
#include "db/sql/abstract_expression.h"
#include "db/sql/simple_sql.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::db::sql::predicate::Predicate;

class SN_CORELIB_EXPORT Where : public Predicate
{
public:
   Where();
};

class SN_CORELIB_EXPORT Having : public Predicate
{
public:
   Having();
};

class SN_CORELIB_EXPORT Literal : public AbstractExpression
{
public:
   Literal(const QString &literal);
   Literal& setLiteral(const QString &literal);
   const QString& getLiteral()const;
public:
   virtual ExpressionDataType getExpressionData()const;
protected:
   QString m_literal;
   
};

}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_SIMPLE_SQL_H
