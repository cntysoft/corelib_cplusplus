#include "predicate.h"
#include "simple_predicate.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace predicate{

Predicate::Predicate()
{}

QSharedPointer<Predicate> Predicate::nest()
{
   QSharedPointer<Predicate> predicate(new Predicate());
   //memory leak 可能的地方
   predicate->setUnnest(QSharedPointer<Predicate>(this));
   QString combineOperator = m_defaultCombination;
   if(!m_nextPredicateCombineOperator.isEmpty()){
      combineOperator = m_nextPredicateCombineOperator;
   }
   addPredicate(predicate, combineOperator);
   m_nextPredicateCombineOperator.clear();
   return predicate;
}

Predicate& Predicate::setUnnest(QSharedPointer<Predicate> predicate)
{
   m_unnest = predicate;
   return *this;
}

QSharedPointer<Predicate> Predicate::unnest()
{
   if(m_unnest.isNull()){
      return m_unnest;
   }
   QSharedPointer<Predicate> unnest = m_unnest;
   m_unnest.clear();
   return unnest;
}

Predicate& Predicate::equalTo(const QString &left, const QVariant &right, 
                              const QString &leftType, const QString &rightType)
{
   QString combineOperator = m_defaultCombination;
   if(!m_nextPredicateCombineOperator.isEmpty()){
      combineOperator = m_nextPredicateCombineOperator;
   }
   addPredicate(PredicatePointerType(new Operator(QVariant(left), Operator::OPERATOR_EQUAL_TO, 
                                                  right, leftType, rightType)), combineOperator);
   m_nextPredicateCombineOperator.clear();
   return *this;
}

Predicate& Predicate::notEqualTo(const QString &left, const QVariant &right, 
                              const QString &leftType, const QString &rightType)
{
   QString combineOperator = m_defaultCombination;
   if(!m_nextPredicateCombineOperator.isEmpty()){
      combineOperator = m_nextPredicateCombineOperator;
   }
   addPredicate(PredicatePointerType(new Operator(QVariant(left), Operator::OPERATOR_NOT_EQUAL_TO, 
                                                  right, leftType, rightType)), combineOperator);
   m_nextPredicateCombineOperator.clear();
   return *this;
}


Predicate& Predicate::lessThan(const QString &left, const QVariant &right, 
                              const QString &leftType, const QString &rightType)
{
   QString combineOperator = m_defaultCombination;
   if(!m_nextPredicateCombineOperator.isEmpty()){
      combineOperator = m_nextPredicateCombineOperator;
   }
   addPredicate(PredicatePointerType(new Operator(QVariant(left), Operator::OPERATOR_LESS_THAN, 
                                                  right, leftType, rightType)), combineOperator);
   m_nextPredicateCombineOperator.clear();
   return *this;
}

Predicate& Predicate::greaterThan(const QString &left, const QVariant &right, 
                              const QString &leftType, const QString &rightType)
{
   QString combineOperator = m_defaultCombination;
   if(!m_nextPredicateCombineOperator.isEmpty()){
      combineOperator = m_nextPredicateCombineOperator;
   }
   addPredicate(PredicatePointerType(new Operator(QVariant(left), Operator::OPERATOR_GREATER_THAN, 
                                                  right, leftType, rightType)), combineOperator);
   m_nextPredicateCombineOperator.clear();
   return *this;
}

Predicate& Predicate::lessThanOrEqualTo(const QString &left, const QVariant &right, 
                              const QString &leftType, const QString &rightType)
{
   QString combineOperator = m_defaultCombination;
   if(!m_nextPredicateCombineOperator.isEmpty()){
      combineOperator = m_nextPredicateCombineOperator;
   }
   addPredicate(PredicatePointerType(new Operator(QVariant(left), Operator::OPERATOR_LESS_THAN_OR_EQUAL_TO, 
                                                  right, leftType, rightType)), combineOperator);
   m_nextPredicateCombineOperator.clear();
   return *this;
}

Predicate& Predicate::greaterThanOrEqualTo(const QString &left, const QVariant &right, 
                              const QString &leftType, const QString &rightType)
{
   QString combineOperator = m_defaultCombination;
   if(!m_nextPredicateCombineOperator.isEmpty()){
      combineOperator = m_nextPredicateCombineOperator;
   }
   addPredicate(PredicatePointerType(new Operator(QVariant(left), Operator::OPERATOR_GREATER_THAN_OR_EQUAL_TO, 
                                                  right, leftType, rightType)), combineOperator);
   m_nextPredicateCombineOperator.clear();
   return *this;
}


Predicate& Predicate::isNull(const QString &identifier)
{
   QString combineOperator = m_defaultCombination;
   if(!m_nextPredicateCombineOperator.isEmpty()){
      combineOperator = m_nextPredicateCombineOperator;
   }
   addPredicate(PredicatePointerType(new IsNull(identifier)), combineOperator);
   m_nextPredicateCombineOperator.clear();
   return *this;
}

Predicate& Predicate::isNotNull(const QString &identifier)
{
   QString combineOperator = m_defaultCombination;
   if(!m_nextPredicateCombineOperator.isEmpty()){
      combineOperator = m_nextPredicateCombineOperator;
   }
   addPredicate(PredicatePointerType(new IsNotNull(identifier)), combineOperator);
   m_nextPredicateCombineOperator.clear();
   return *this;
}

Predicate& Predicate::like(const QString &identifier, const QString &like)
{
   QString combineOperator = m_defaultCombination;
   if(!m_nextPredicateCombineOperator.isEmpty()){
      combineOperator = m_nextPredicateCombineOperator;
   }
   addPredicate(PredicatePointerType(new Like(identifier, like)), combineOperator);
   m_nextPredicateCombineOperator.clear();
   return *this;
}

Predicate& Predicate::notLike(const QString &identifier, const QString &like)
{
   QString combineOperator = m_defaultCombination;
   if(!m_nextPredicateCombineOperator.isEmpty()){
      combineOperator = m_nextPredicateCombineOperator;
   }
   addPredicate(PredicatePointerType(new NotLike(identifier, like)), combineOperator);
   m_nextPredicateCombineOperator.clear();
   return *this;
}

Predicate& Predicate::predicate(QSharedPointer<Predicate> predicate)
{
   QString combineOperator = m_defaultCombination;
   if(!m_nextPredicateCombineOperator.isEmpty()){
      combineOperator = m_nextPredicateCombineOperator;
   }
   addPredicate(predicate, combineOperator);
   m_nextPredicateCombineOperator.clear();
   return *this;
}


Predicate& Predicate::expression(const QString &expression, const QList<QVariant> &parameters)
{
   QString combineOperator = m_defaultCombination;
   if(!m_nextPredicateCombineOperator.isEmpty()){
      combineOperator = m_nextPredicateCombineOperator;
   }
   addPredicate(PredicatePointerType(new Expression(expression, parameters)), combineOperator);
   m_nextPredicateCombineOperator.clear();
   return *this;
}

Predicate& Predicate::literal(const QString &literal)
{
   QString combineOperator = m_defaultCombination;
   if(!m_nextPredicateCombineOperator.isEmpty()){
      combineOperator = m_nextPredicateCombineOperator;
   }
   addPredicate(PredicatePointerType(new Literal(literal)), combineOperator);
   m_nextPredicateCombineOperator.clear();
   return *this;
}

Predicate& Predicate::in(const QString &identifier, const QList<QVariant> &valueSet)
{
   QString combineOperator = m_defaultCombination;
   if(!m_nextPredicateCombineOperator.isEmpty()){
      combineOperator = m_nextPredicateCombineOperator;
   }
   addPredicate(PredicatePointerType(new In(identifier, valueSet)));
   m_nextPredicateCombineOperator.clear();
   return *this;
}

Predicate& Predicate::notIn(const QString &identifier, const QList<QVariant> &valueSet)
{
   QString combineOperator = m_defaultCombination;
   if(!m_nextPredicateCombineOperator.isEmpty()){
      combineOperator = m_nextPredicateCombineOperator;
   }
   addPredicate(PredicatePointerType(new NotIn(identifier, valueSet)));
   m_nextPredicateCombineOperator.clear();
   return *this;
}

Predicate& Predicate::between(const QString &identifier, const QVariant &minValue, const QVariant &maxValue)
{
   QString combineOperator = m_defaultCombination;
   if(!m_nextPredicateCombineOperator.isEmpty()){
      combineOperator = m_nextPredicateCombineOperator;
   }
   addPredicate(PredicatePointerType(new Between(identifier, minValue, maxValue)));
   m_nextPredicateCombineOperator.clear();
   return *this;
}

Predicate& Predicate::setOrCombination()
{
   m_nextPredicateCombineOperator = PredicateSet::COMBINED_BY_OR;
   return *this;
}

Predicate& Predicate::setAndCombination()
{
   m_nextPredicateCombineOperator = PredicateSet::COMBINED_BY_AND;
   return *this;
}

}//predicate
}//sql
}//db
}//corelib
}//sn
