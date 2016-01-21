#include "simple_predicate.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace predicate{

Between::Between(const QString &identifier, const QString &minValue, const QString &maxValue)
   :m_specification("%1$s BETWEEN %2$s AND %3$s")
{
   if(!identifier.isNull()){
      setIdentifier(identifier);
   }
   if(!minValue.isNull()){
      setMinValue(minValue);
   }
   if(!maxValue.isNull()){
      setMaxValue(maxValue);
   }
}

Between& Between::setIdentifier(const QString &identifier)
{
  m_identifier = identifier;
  return *this;
}

const QString& Between::getIdentifier()
{
   return m_identifier;
}

Between& Between::setMinValue(const QString &minValue)
{
   m_minValue = minValue;
   return *this;
}

const QString& Between::getMinValue()
{
   return m_minValue;
}

Between& Between::setMaxValue(const QString &maxValue)
{
   m_maxValue = maxValue;
   return *this;
}

const QString& Between::getMaxValue()
{
   return m_maxValue;
}

Between& Between::setSpecification(const QString &specification)
{
   m_specification = specification;
   return *this;
}

const QString& Between::getSpecification()
{
   return m_specification;
}

Where::Where()
{}

}//predicate
}//sql
}//db
}//corelib
}//sn