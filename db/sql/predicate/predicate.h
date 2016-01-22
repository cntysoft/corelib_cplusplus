#ifndef SN_CORELIB_DB_SQL_PREDICATE_PREDICATE_H
#define SN_CORELIB_DB_SQL_PREDICATE_PREDICATE_H

#include <QSharedPointer>
#include <QString>

#include "global/global.h"
#include "kernel/errorinfo.h"
#include "db/sql/predicate/predicateset.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace predicate{

using sn::corelib::ErrorInfo;

class SN_CORELIB_EXPORT Predicate : public PredicateSet
{
public:
   Predicate();
   QSharedPointer<Predicate> nest();
   Predicate& setUnnest(QSharedPointer<Predicate> predicate);
   QSharedPointer<Predicate> unnest();
   Predicate& equalTo(const QString &left, const QVariant &right, 
                      const QString &leftType = AbstractExpression::TYPE_IDENTIFIER, 
                      const QString &rightType = AbstractExpression::TYPE_VALUE);
   Predicate& isNull(const QString &identifier);
   Predicate& setOrCombination();
   Predicate& setAndCombination();
protected:
   QSharedPointer<Predicate> m_unnest;
   QString m_nextPredicateCombineOperator;
};

}//predicate
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_PREDICATE_PREDICATE_H

