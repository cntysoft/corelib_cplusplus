#include "predicateset.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace predicate{

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