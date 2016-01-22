#include "abstract_expression.h"
#include "kernel/errorinfo.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::ErrorInfo;

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

AbstractExpression::ExpressionDataType AbstractExpression::getExpressionData()const
{
   return {};
}

QPair<QVariant, QString> AbstractExpression::normalizeArgument(const QVariant &argument, const QString &argumentType)const
{
   if(!m_allowTypes.contains(argumentType)){
      throw ErrorInfo(QString("Argument type should be in array(%1)").arg(m_allowTypes.join(',')));
   }
   return {argument, argumentType};
}

AbstractExpression::~AbstractExpression()
{}

}//sql
}//db
}//corelib
}//sn