#include <QVariant>
#include <QStringList>

#include "simple_sql.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

Where::Where()
{}

Having::Having()
{}

Literal::Literal(const QString &literal)
   : m_literal(literal)
{
}

const QString& Literal::getLiteral() const
{
   return m_literal;
}

Literal& Literal::setLiteral(const QString &literal)
{
   m_literal = literal;
   return *this;
}

AbstractExpression::ExpressionDataType Literal::getExpressionData()const
{
   QString literal(m_literal);
   return {
      QVariant(literal.replace("%", "%%")),
            QVariant(QList<QVariant>()),
            QVariant(QStringList())
   };
}

}//sql
}//db
}//corelib
}//sn