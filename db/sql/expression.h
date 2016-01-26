#ifndef SN_CORELIB_DB_SQL_EXPRESSION_H
#define SN_CORELIB_DB_SQL_EXPRESSION_H

#include <QChar>
#include <QString>
#include <QList>
#include <QVariant>
#include <QSharedPointer>

#include "abstract_expression.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

class SN_CORELIB_EXPORT Expression : public AbstractExpression
{
public:
   const static QChar PLACEHOLDER;
public:
   Expression(const QString &expression, const QList<QVariant> &parameters = QList<QVariant>());
   Expression(const Expression& other) = default;
   Expression& setExpression();
   const QString& getExpression()const;
   Expression& setExpression(const QString &expression);
   Expression& setParameters(const QList<QVariant> &parameters);
   const QList<QVariant>& getParameters()const;
public:
   virtual AbstractExpression::ExpressionDataType getExpressionData()const;
public:
   QString m_expression;
   QList<QVariant> m_parameters;
};

}//sql
}//db
}//corelib
}//sn

Q_DECLARE_METATYPE(QSharedPointer<sn::corelib::db::sql::Expression>)

#endif // SN_CORELIB_DB_SQL_EXPRESSION_H