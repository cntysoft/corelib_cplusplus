#include <QPair>

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

//IsNull

IsNull::IsNull(const QString &identifier)
   : m_identifier(identifier)
{
}

IsNull& IsNull::setIdentifier(const QString &identifier)
{
   m_identifier = identifier;
   return *this;
}

const QString& IsNull::getIdentifier()const
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
   QPair<QVariant, QString> identifier = normalizeArgument(m_identifier, AbstractExpression::TYPE_IDENTIFIER);
   QList<QVariant>  values;
   QList<QVariant>  types;
   values.append(identifier.first);
   types.append(QVariant(identifier.second));
   return {
      QVariant(getSpecification()),
            QVariant(values),
            QVariant(types)
   };
}

//Operator

const  QLatin1String Operator::OPERATOR_EQUAL_TO("=");
const  QLatin1String Operator::OP_EQ("=");
const  QLatin1String Operator::OPERATOR_NOT_EQUAL_TO("!=");
const  QLatin1String Operator::OP_NE("!=");
const  QLatin1String Operator::OPERATOR_LESS_THAN("<");
const  QLatin1String Operator::OP_LT("<");
const  QLatin1String Operator::OPERATOR_LESS_THAN_OR_EQUAL_TO("<=");
const  QLatin1String Operator::OP_LTE("<=");
const  QLatin1String Operator::OPERATOR_GREATER_THAN(">");
const  QLatin1String Operator::OP_GT(">");
const  QLatin1String Operator::OPERATOR_GREATER_THAN_OR_EQUAL_TO(">=");
const  QLatin1String Operator::OP_GTE(">=");

Operator::Operator(const QVariant &left, const QLatin1String &operatorType, const QVariant &right, const QString &leftType, const QString &rightType)
   : m_left(left),
     m_right(right),
     m_operatorType(Operator::OPERATOR_EQUAL_TO),
     m_leftType(AbstractExpression::TYPE_IDENTIFIER),
     m_rightType(AbstractExpression::TYPE_VALUE)
{
   if(!leftType.isNull()){
      setLeftType(leftType);
   }
   if(!rightType.isNull()){
      setRightType(rightType);
   }
   if(operatorType != Operator::OPERATOR_EQUAL_TO){
      setOperator(operatorType);
   }
   if(leftType != AbstractExpression::TYPE_IDENTIFIER){
      setLeftType(leftType);
   }
   m_allowTypes.append(AbstractExpression::TYPE_IDENTIFIER);
   m_allowTypes.append(AbstractExpression::TYPE_VALUE);
}

Operator& Operator::setLeft(const QVariant &left)
{
   m_left = left;
   return *this;
}

const QVariant& Operator::getLeft() const
{
   return m_left;
}

Operator& Operator::setLeftType(const QString &type)throw(ErrorInfo)
{
   if(!m_allowTypes.contains(type)){
      throw ErrorInfo(QString("Invalid type \"%1\" provided; must be of type \"%2\" or \"%3\"")
                      .arg(type)
                      .arg("AbstractExpression::TYPE_IDENTIFIER")
                      .arg("AbstractExpression::TYPE_VALUE"));
   }
   m_leftType = type;
   return *this;
}

const QString& Operator::getLeftType() const
{
   return m_leftType;
}

Operator& Operator::setOperator(const QLatin1String &operatorType)
{
   m_operatorType = operatorType;
   return *this;
}

const QLatin1String& Operator::getOperator() const
{
   return m_operatorType;
}

Operator& Operator::setRightType(const QString &type)throw(ErrorInfo)
{
   if(!m_allowTypes.contains(type)){
      throw ErrorInfo(QString("Invalid type \"%1\" provided; must be of type \"%2\" or \"%3\"")
                      .arg(type)
                      .arg("AbstractExpression::TYPE_IDENTIFIER")
                      .arg("AbstractExpression::TYPE_VALUE"));
   }
   m_rightType = type;
   return *this;
}

const QString& Operator::getRightType() const
{
   return m_rightType;
}

AbstractExpression::ExpressionDataType Operator::getExpressionData()const
{
   QList<QVariant> values;
   QList<QVariant> types;
   QPair<QVariant, QString> left = normalizeArgument(m_left, m_leftType);
   QPair<QVariant, QString> right = normalizeArgument(m_right, m_rightType);
   values << left.first << right.first;
   types << QVariant(left.second) << QVariant(right.second);
   return {
      QVariant("%1 "+m_operatorType+" %2"),
            values,
            types
   };
}

}//predicate
}//sql
}//db
}//corelib
}//sn