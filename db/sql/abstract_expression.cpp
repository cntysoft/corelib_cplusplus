#include "abstract_expression.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

const QString AbstractExpression::TYPE_IDENTIFIER = "identifier";
const QString AbstractExpression::TYPE_VALUE = "value";
const QString AbstractExpression::TYPE_LITERAL = "literal";
const QString AbstractExpression::TYPE_SELECT = "select";

AbstractExpression::AbstractExpression()
{
   m_allowTypes.append(AbstractExpression::TYPE_IDENTIFIER);
   m_allowTypes.append(AbstractExpression::TYPE_LITERAL);
   m_allowTypes.append(AbstractExpression::TYPE_SELECT);
   m_allowTypes.append(AbstractExpression::TYPE_VALUE);
}

QList<QMap<QString, QVariant>> AbstractExpression::getExpressionData()const
{}

AbstractExpression::~AbstractExpression()
{}

}//sql
}//db
}//corelib
}//sn