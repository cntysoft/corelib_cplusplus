#ifndef SN_CORELIB_DB_SQL_PREDICATE_SIMPLE_PREDICATE_H
#define SN_CORELIB_DB_SQL_PREDICATE_SIMPLE_PREDICATE_H

#include <QString>
#include <QLatin1String>

#include "global/global.h"
#include "kernel/errorinfo.h"
#include "db/sql/abstract_expression.h"
#include "db/sql/predicate/predicateset.h"
#include "db/sql/predicate/predicate.h"
#include "db/sql/literal.h"
#include "db/sql/expression.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace predicate{

using BaseLiteral = sn::corelib::db::sql::Literal;
using sn::corelib::db::sql::AbstractExpression;
using BaseExpression = sn::corelib::db::sql::Expression;
using sn::corelib::ErrorInfo;

class SN_CORELIB_EXPORT Between : public AbstractExpression
{
   Q_OBJECT
public:
   Between(const QString &identifier = QString(), const QString &minValue = QString(), const QString &maxValue = QString());
   Between& setIdentifier(const QString &identifier);
   const QString& getIdentifier();
   Between& setMinValue(const QString &minValue);
   const QString& getMinValue();
   Between& setMaxValue(const QString &maxValue);
   const QString& getMaxValue();
   Between& setSpecification(const QString &specification);
   const QString& getSpecification()const;
public:
   virtual ExpressionDataType getExpressionData()const;
protected:
   QString m_specification;
   QString m_identifier;
   QString m_minValue;
   QString m_maxValue;
};



class SN_CORELIB_EXPORT In : public AbstractExpression
{
   Q_OBJECT
public:
   In(const QString &identifier = QString(), const QVariant &valueSet = QVariant());
   In& setIdentifier(const QString &identifier);
   const QString& getIdentifier()const;
   In& setValueSet(const QStringList &valueSet);
   const QVariant& getValueSet()const;
public:
   virtual ExpressionDataType getExpressionData()const;
protected:
   QString m_specification;
   QString m_valueSpecSpecification;
   QString m_identifier;
   QVariant m_valueSet;
};

class SN_CORELIB_EXPORT Where : public Predicate
{
   Q_OBJECT
public:
   Where();
};

class SN_CORELIB_EXPORT Literal : public BaseLiteral
{
   Q_OBJECT
public:
   Literal(const QString &literal);
};

class SN_CORELIB_EXPORT Expression : public BaseExpression
{
   Q_OBJECT
public:
   Expression(const QString &expression, const QStringList &parameters = QStringList());
};

class SN_CORELIB_EXPORT IsNull : public AbstractExpression
{
   Q_OBJECT
public:
   IsNull(const QString &identifier = QString());
   IsNull& setIdentifier(const QString &identifier);
   const QString& getIdentifier()const;
   IsNull& setSpecification(const QString &specification);
   const QString& getSpecification()const;
public:
   virtual ExpressionDataType getExpressionData()const;
protected:
   QString m_specification;
   QString m_identifier;
};

class SN_CORELIB_EXPORT IsNotNull : public IsNull
{
public:
   IsNotNull(const QString &identifier = QString());
};

class SN_CORELIB_EXPORT Operator : public AbstractExpression
{
   Q_OBJECT
public:
   const static QLatin1String OPERATOR_EQUAL_TO;
   const static QLatin1String OP_EQ;
   const static QLatin1String OPERATOR_NOT_EQUAL_TO;
   const static QLatin1String OP_NE;
   const static QLatin1String OPERATOR_LESS_THAN;
   const static QLatin1String OP_LT;
   const static QLatin1String OPERATOR_LESS_THAN_OR_EQUAL_TO;
   const static QLatin1String OP_LTE;
   const static QLatin1String OPERATOR_GREATER_THAN;
   const static QLatin1String OP_GT;
   const static QLatin1String OPERATOR_GREATER_THAN_OR_EQUAL_TO;
   const static QLatin1String OP_GTE;
public:
   Operator(const QVariant &left = QVariant(), const QLatin1String &operatorType = Operator::OPERATOR_EQUAL_TO,
            const QVariant &right = QVariant(), const QString &leftType = AbstractExpression::TYPE_IDENTIFIER,
            const QString &rightType = AbstractExpression::TYPE_VALUE);
   Operator& setLeft(const QVariant &left);
   const QVariant& getLeft()const;
   Operator& setLeftType(const QString &type)throw(ErrorInfo);
   const QString& getLeftType()const;
   Operator& setOperator(const QLatin1String &operatorType);
   const QLatin1String& getOperator()const;
   Operator& setRight(const QVariant &right);
   const QVariant& getRight()const;
   Operator& setRightType(const QString &type)throw(ErrorInfo);
   const QString& getRightType()const;
public:
   virtual ExpressionDataType getExpressionData()const;
protected:
   QVariant m_left;
   QVariant m_right;
   QString m_leftType;
   QString m_rightType;
   QLatin1String m_operatorType;
};

class SN_CORELIB_EXPORT Like : public AbstractExpression
{
public:
   Like(const QString &identifier = QString(), const QString &like = QString());
   Like& setIdentifier(const QString &identifier);
   const QString& getIdentifier()const;
   Like& setLike(const QString &like);
   const QString& getLike()const;
   Like& setSpecification(const QString &specification);
   const QString& getSpecification()const;
public:
   virtual ExpressionDataType getExpressionData()const;
protected:
   QString m_identifier;
   QString m_like;
   QString m_specification;
};

}//predicate
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_PREDICATE_SIMPLE_PREDICATE_H
