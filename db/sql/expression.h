#ifndef SN_CORELIB_DB_SQL_EXPRESSION_H
#define SN_CORELIB_DB_SQL_EXPRESSION_H

#include <QChar>
#include <QString>
#include <QStringList>

#include "abstract_expression.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

class SN_CORELIB_EXPORT Expression : public AbstractExpression
{
   Q_OBJECT
public:
   const static QChar PLACEHOLDER;
public:
   Expression(const QString &expression, const QStringList &parameters = QStringList());
   Expression& setExpression();
   const QString& getExpression()const;
   Expression& setExpression(const QString &expression);
   Expression& setParameters(const QStringList &parameters);
   const QStringList& getParameters()const;
public:
   virtual AbstractExpression::ExpressionDataType getExpressionData()const;
protected:
   QString m_expression;
   QStringList m_parameters;
};

}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_EXPRESSION_H