#ifndef SN_CORELIB_DB_SQL_PREDICATE_SIMPLE_PREDICATE_H
#define SN_CORELIB_DB_SQL_PREDICATE_SIMPLE_PREDICATE_H

#include <QString>

#include "global/global.h"
#include "db/sql/abstract_expression.h"
#include "db/sql/predicate/predicateset.h"
#include "db/sql/literal.h"
#include "db/sql/expression.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace predicate{

using BaseLiteral = sn::corelib::db::sql::Literal;
using sn::corelib::db::sql::AbstractExpression;
using BaseExpression = sn::corelib::db::sql::Expression;

class SN_CORELIB_EXPORT Between : public AbstractExpression
{
public:
   Between(const QString &identifier = QString(), const QString &minValue = QString(), const QString &maxValue = QString());
   Between& setIdentifier(const QString &identifier);
   const QString& getIdentifier();
   Between& setMinValue(const QString &minValue);
   const QString& getMinValue();
   Between& setMaxValue(const QString &maxValue);
   const QString& getMaxValue();
   Between& setSpecification(const QString &specification);
   const QString& getSpecification();
protected:
   QString m_specification;
   QString m_identifier;
   QString m_minValue;
   QString m_maxValue;
};


class SN_CORELIB_EXPORT Where : public PredicateSet
{
public:
   Where();
};

class SN_CORELIB_EXPORT Literal : public BaseLiteral
{
public:
   Literal(const QString &literal);
};

class SN_CORELIB_EXPORT Expression : public BaseExpression
{
public:
   Expression(const QString &expression, const QStringList &parameters = QStringList());
};

class SN_CORELIB_EXPORT IsNull : public AbstractExpression
{
public:
   IsNull(const QString &identifier = QString());
   IsNull& setIdentifier(const QString &identifier);
   const QString& getIdentifier()const;
   IsNull& setSpecification(const QString &specification);
   const QString& getSpecification()const;
public:
   virtual ExpressionDataType getExpressionData()const;
protected:
   QString m_specification;
   QString m_identifier;
};

}//predicate
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_PREDICATE_SIMPLE_PREDICATE_H
