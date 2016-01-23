#include "global/common_funcs.h"
#include "predicateset.h"
#include "simple_predicate.h"

#include <QDebug>

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace predicate{

using sn::corelib::instanceof;

const QString PredicateSet::COMBINED_BY_AND = "AND";
const QString PredicateSet::OP_AND = "AND";
const QString PredicateSet::COMBINED_BY_OR = "OR";
const QString PredicateSet::OP_OR = "OR";

const QStringList PredicateSet::sm_supportedCombinations{
   PredicateSet::OP_AND, PredicateSet::OP_OR
};

PredicateSet::PredicateSet(const QList<PredicatePointerType> &predicates, const QString &defaultCombination)
   :m_defaultCombination(defaultCombination)
{
   if(!predicates.isEmpty()){
      QList<PredicatePointerType>::const_iterator it = predicates.cbegin();
      while(it != predicates.cend()){
         addPredicate(*it);
         it++;
      }
   }   
}

AbstractExpression::ExpressionDataType PredicateSet::getExpressionData()const
{
   ExpressionDataType parts;
   int count = m_predicates.size();
   for(int i = 0; i < count; i++){
      PredicatePointerType predicate = m_predicates[i].second;
      if(instanceof<PredicateSet>(predicate.data())){
         parts << QVariant("(");
      }
      parts << QVariant(predicate->getExpressionData());
      if(instanceof<PredicateSet>(predicate.data())){
         parts << QVariant(")");
      }
      int next = i + 1;
      if(next < count){
         parts << QVariant(QString(" %1 ").arg(m_predicates[next].first));
      }
   }
   return parts;
}

PredicateSet& PredicateSet::addPredicate(PredicatePointerType predicate, QString combination)
{
   if(combination.isNull() || !PredicateSet::sm_supportedCombinations.contains(combination)){
      combination = m_defaultCombination;
   }
   if(PredicateSet::OP_OR == combination){
      orPredicate(predicate);
      return *this;
   }
   andPredicate(predicate);
   return *this;
}

PredicateSet& PredicateSet::addPredicate(const QString &predicate, QString combination)
{
   PredicateSet::PredicatePointerType predicateObj;
   if(predicate.indexOf(Expression::PLACEHOLDER) != -1){
      predicateObj.reset(new Expression(predicate));
   }else{
      predicateObj.reset(new Literal(predicate));
   }
   addPredicate(predicateObj, combination);
   return *this;
}

PredicateSet& PredicateSet::addPredicates(const QList<PredicatePointerType> &predicates, QString combination)
{
   QList<PredicatePointerType>::const_iterator it = predicates.cbegin();
   while(it != predicates.cend()){
      addPredicate(*it, combination);
      it++;
   }
   return *this;
}

PredicateSet& PredicateSet::addPredicates(const QList<QString> &predicates, QString combination)
{
   QList<QString>::const_iterator it = predicates.cbegin();
   while(it != predicates.cend()){
      QString predicate(*it);
      PredicateSet::PredicatePointerType predicateObj;
      if(predicate.indexOf(Expression::PLACEHOLDER) != -1){
         predicateObj.reset(new Expression(predicate));
      }else{
         predicateObj.reset(new Literal(predicate));
      }
      addPredicate(predicateObj, combination);
      it++;
   }
   return *this;
}

PredicateSet& PredicateSet::addPredicates(const QMap<QString, QVariant> &predicates, QString combination)
{
   QMap<QString, QVariant>::const_iterator it = predicates.cbegin();
   while(it != predicates.cend()){
      PredicateSet::PredicatePointerType predicate;
      QString key(it.key());
      const QVariant &value = it.value();
      if(key.indexOf('?') != -1){
         // First, process strings that the abstraction replacement character ?
         // as an Expression predicate
         predicate.reset(new Expression(key, value.toList()));
      }else if(value.isNull()){
         // map PHP null to SQL IS NULL expression
         predicate.reset(new IsNull(key));
      }else if(value.canConvert(QMetaType::QStringList)){
         predicate.reset(new In(key, value));
      }else if(value.canConvert(QMetaType::QString)){
         QString strValue = value.toString();
         if(strValue.indexOf(Expression::PLACEHOLDER) != -1){
            predicate.reset(new Expression(strValue));
         }else{
            predicate.reset(new Literal(strValue));
         }
      }
      addPredicate(predicate, combination);
      it++;
   }
   return *this;
}

PredicateSet& PredicateSet::andPredicate(PredicatePointerType predicate)
{
   m_predicates.append(PredicateItemType(PredicateSet::OP_AND, predicate));
   return *this;
}

PredicateSet& PredicateSet::orPredicate(PredicatePointerType predicate)
{
   m_predicates.append(PredicateItemType(PredicateSet::OP_OR, predicate));
   return *this;
}

const PredicateSet::PredicateListType& PredicateSet::getPredicates()
{
   return m_predicates;
}

PredicateSet& PredicateSet::clearPredicates()
{
   m_predicates.clear();
   return *this;
}

int PredicateSet::count()
{
   return m_predicates.size();
}

PredicateSet::~PredicateSet()
{}

}//predicate
}//sql
}//db
}//corelib
}//sn