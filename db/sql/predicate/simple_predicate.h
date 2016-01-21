#ifndef SN_CORELIB_DB_SQL_PREDICATE_BETWEEN_H
#define SN_CORELIB_DB_SQL_PREDICATE_BETWEEN_H

#include "global/global.h"
#include "db/sql/abstract_expression.h"
#include "db/sql/predicate/predicateset.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace predicate{

using sn::corelib::db::sql::AbstractExpression;

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

}//predicate
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_PREDICATE_BETWEEN_H
