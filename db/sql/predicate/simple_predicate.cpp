#include <QPair>

#include "simple_predicate.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace predicate{

Between::Between(const QString &identifier, const QVariant &minValue, const QVariant &maxValue)
   : m_specification("%1 BETWEEN %2 AND %3"),
     m_identifier(identifier)
{
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

Between& Between::setMinValue(const QVariant &minValue)
{
   m_minValue = minValue;
   return *this;
}

const QVariant& Between::getMinValue()
{
   return m_minValue;
}

Between& Between::setMaxValue(const QVariant &maxValue)
{
   m_maxValue = maxValue;
   return *this;
}

const QVariant& Between::getMaxValue()
{
   return m_maxValue;
}

Between& Between::setSpecification(const QString &specification)
{
   m_specification = specification;
   return *this;
}

const QString& Between::getSpecification() const
{
   return m_specification;
}


AbstractExpression::ExpressionDataType Between::getExpressionData()const
{
   QPair<QVariant, QString> identifier = normalizeArgument(m_identifier, AbstractExpression::TYPE_IDENTIFIER);
   QPair<QVariant, QString> minValue = normalizeArgument(m_minValue, AbstractExpression::TYPE_VALUE);
   QPair<QVariant, QString> maxValue = normalizeArgument(m_maxValue, AbstractExpression::TYPE_VALUE);
   QList<QVariant>  values;
   QList<QVariant>  types;
   values << identifier.first << minValue.first << maxValue.first;
   types << QVariant(identifier.second) << QVariant(minValue.second) << QVariant(maxValue.second);
   return {
      QVariant(getSpecification()),
            QVariant(values),
            QVariant(types)
   };
}

In::In(const QString &identifier, const QList<QVariant> &valueSet)
   : In(QVariant(identifier), QVariant(valueSet))
{
}

In::In(const QStringList &identifier, const QList<QVariant> &valueSet)
   : In(QVariant(identifier), QVariant(valueSet))
{
}

In::In(const QVariant &identifier, const QVariant &valueSet)
   : m_identifier(identifier),
     m_valueSet(valueSet),
     m_specification("%1 IN %2")
{}

In& In::setIdentifier(const QString &identifier)
{
   if(!m_identifier.isNull()){
      m_identifier.clear();
   }
   m_identifier.setValue(identifier);
   return *this;
}

In& In::setIdentifier(const QStringList &identifier)
{
   if(!m_identifier.isNull()){
      m_identifier.clear();
   }
   m_identifier.setValue(identifier);
   return *this;
}

const QVariant& In::getIdentifier() const
{
   return m_identifier;
}

In& In::setValueSet(const QList<QVariant> &valueSet)
{
   if(!m_valueSet.isNull()){
      m_valueSet.clear();
   }
   m_valueSet.setValue(valueSet);
   return *this;
}

const QList<QVariant> In::getValueSet() const
{
   return m_valueSet.toList();
}

AbstractExpression::ExpressionDataType In::getExpressionData()const
{
   QList<QVariant> values;
   QStringList types;
   QString identifierSpecFragment;
   QString specification;
   int serial = 1;
   if(m_identifier.type() == QVariant::List){
      QStringList identifiers = m_identifier.toStringList();
      int identifierCount = identifiers.size();
      QStringList parts;
      for(int i = 0; i < identifierCount; i++){
         parts << QString("%%1").arg(serial);
         values << QVariant(identifiers.at(i));
         types << AbstractExpression::TYPE_IDENTIFIER;
         serial++;
      }
      identifierSpecFragment = QString("(%1)").arg(parts.join(", "));
   }else{
      identifierSpecFragment = "%1";
      serial++;
      values << m_identifier;
      types << AbstractExpression::TYPE_IDENTIFIER;
   }
   //subselect
   //   if ($values instanceof Select) {
   //               $specification = vsprintf(
   //                   $this->specification,
   //                   array($identifierSpecFragment, '%s')
   //               );
   //               $replacements[] = $values;
   //               $types[] = self::TYPE_VALUE;
   //           } else {
   if(m_valueSet.type() == QVariant::List){
      QList<QVariant> valueSet = m_valueSet.toList();
      int valueSetCount = valueSet.size();
      QString valueTpl;
      QStringList valueParts;
      for(int i = 0; i < valueSetCount; i++){
         QPair<QVariant, QString> item = normalizeArgument(valueSet.at(i), AbstractExpression::TYPE_VALUE);
         values << item.first;
         types << item.second;
         valueParts << QString("%%1").arg(serial);
         serial++;
      }
      valueTpl = "("+valueParts.join(", ")+")";
      specification = m_specification.arg(identifierSpecFragment, valueTpl);
   }
   return {
      specification,
            QVariant(values),
            QVariant(types)
            
   };
}

NotIn::NotIn(const QString &identifier, const QList<QVariant> &valueSet)
   : In(identifier, valueSet)
{
   m_specification = "%1 NOT IN %2";
}

NotIn::NotIn(const QStringList &identifier, const QList<QVariant> &valueSet)
   : In(identifier, valueSet)
{
   m_specification = "%1 NOT IN %2";
}

Where::Where()
{}

Literal::Literal(const QString &literal)
   : BaseLiteral(literal)
{}

Expression::Expression(const QString &expression, const QList<QVariant> &parameters)
   : BaseExpression(expression, parameters)
{}

//IsNull

IsNull::IsNull(const QString &identifier)
   : m_identifier(identifier),
     m_specification("%1 IS NULL")
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
   QStringList  types;
   values.append(identifier.first);
   types.append(identifier.second);
   return {
      QVariant(getSpecification()),
            QVariant(values),
            QVariant(types)
   };
}

//IsNotNull

IsNotNull::IsNotNull(const QString &identifier)
   : IsNull(identifier)
{
   m_specification = "%1 IS NOT NULL";
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
     m_leftType(AbstractExpression::TYPE_IDENTIFIER),
     m_rightType(AbstractExpression::TYPE_VALUE),
     m_operatorType(Operator::OPERATOR_EQUAL_TO)
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
   m_allowTypes.clear();
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
   QStringList types;
   QPair<QVariant, QString> left = normalizeArgument(m_left, m_leftType);
   QPair<QVariant, QString> right = normalizeArgument(m_right, m_rightType);
   values << left.first << right.first;
   types << left.second << right.second;
   return {
      QVariant("%1 "+m_operatorType+" %2"),
            QVariant(values),
            QVariant(types)
   };
}

//Like

Like::Like(const QString &identifier, const QString &like)
   : m_identifier(identifier),
     m_like(like),
     m_specification("%1 LIKE %2")
{
}

Like& Like::setLike(const QString &like)
{
   m_like = like;
   return *this;
}

const QString& Like::getLike()const
{
   return m_like;
}

Like& Like::setIdentifier(const QString &identifier)
{
   m_identifier = identifier;
   return *this;
}

const QString& Like::getIdentifier() const
{
   return m_identifier;
}

Like& Like::setSpecification(const QString &specification)
{
   m_specification = specification;
   return *this;
}

const QString& Like::getSpecification() const
{
   return m_specification;
}

AbstractExpression::ExpressionDataType Like::getExpressionData()const
{
   QList<QVariant> values;
   QStringList types;
   QPair<QVariant, QString> identifier = normalizeArgument(m_identifier, AbstractExpression::TYPE_IDENTIFIER);
   QPair<QVariant, QString> like = normalizeArgument(m_like, AbstractExpression::TYPE_VALUE);
   values << identifier.first << like.first;
   types << identifier.second << like.second;
   return {
      QVariant(m_specification),
            QVariant(values),
            QVariant(types)
   };
}

//NotLike

NotLike::NotLike(const QString &identifier, const QString &like)
   : Like(identifier, like)
{
   m_specification = "%1 NOT LIKE %2";
}

}//predicate
}//sql
}//db
}//corelib
}//sn