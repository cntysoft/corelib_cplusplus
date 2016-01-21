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

Literal::Literal(const QString &literal)
   : BaseLiteral(literal)
{}

Expression::Expression(const QString &expression, const QStringList &parameters)
   : BaseExpression(expression, parameters)
{}

IsNull::IsNull(const QString &identifier)
   : m_identifier(identifier)
{
}

IsNull& IsNull::setIdentifier(const QString &identifier)
{
   m_identifier = identifier;
   return *this;
}

const QString IsNull::getIdentifier()const
{
   return m_identifier;
}

IsNull& IsNull::setSpecification(const QString &specification)
{
   m_specification = specification;
   return *this;
}

const QString& IsNull::getSpecification() const
{
   return m_specification;
}

AbstractExpression::ExpressionDataType IsNull::getExpressionData()const
{
   QPair<QString, QString> identifier = normalizeArgument(m_identifier, AbstractExpression::TYPE_IDENTIFIER);
   return {
      QVariant(getSpecification()),
      QVariant(identifier.first),
      QVariant(identifier.second)
   };
}

}//predicate
}//sql
}//db
}//corelib
}//sn