#ifndef SN_CORELIB_DB_SQL_PREDICATE_PREDICATE_H
#define SN_CORELIB_DB_SQL_PREDICATE_PREDICATE_H

#include <QSharedPointer>
#include <QString>

#include "global/global.h"
#include "db/sql/predicate/predicateset.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace predicate{

class SN_CORELIB_EXPORT Predicate : public PredicateSet
{
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

