#include <QVariant>

#include "literal.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

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
            QVariant(),
            QVariant()
   };
}

}//sql
}//db
}//corelib
}//sn