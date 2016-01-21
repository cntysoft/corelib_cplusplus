#ifndef SN_CORELIB_DB_SQL_PREDICATEPREDICATESET_H
#define SN_CORELIB_DB_SQL_PREDICATEPREDICATESET_H

#include <QString>
#include <QSharedPointer>
#include <QList>
#include <QPair>

#include "db/sql/abstract_expression.h"
#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace predicate{

using sn::corelib::db::sql::AbstractExpression;

class SN_CORELIB_EXPORT PredicateSet : public AbstractExpression
{
public:
   const static QString COMBINED_BY_AND;
   const static QString OP_AND;
   const static QString COMBINED_BY_OR;
   const static QString OP_OR;
   using PredicatePointerType = QSharedPointer<AbstractExpression>;
   using PredicateItemType = QPair<QString, PredicatePointerType>;
   using PredicateListType = QList<PredicateItemType>;
public:
   PredicateSet(const QList<PredicatePointerType> &predicates = QList<PredicatePointerType>(), 
                const QString &defaultCombination = PredicateSet::COMBINED_BY_AND);
   PredicateSet& addPredicate(PredicatePointerType predicate, QString combination = QString());
   PredicateSet& orPredicate(PredicatePointerType predicate);
   PredicateSet& andPredicate(PredicatePointerType predicate);
   const PredicateListType& getPredicates();
   int count();
public:
   virtual ExpressionDataType getExpressionData()const;
public:
   virtual ~PredicateSet();
protected:
   QString m_defaultCombination;
   PredicateListType m_predicates;
   const static QStringList sm_supportedCombinations;
};

}//predicate
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_PREDICATEPREDICATESET_H

