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

const QStringList AbstractExpression::sm_allowTypes{
   AbstractExpression::TYPE_IDENTIFIER,
         AbstractExpression::TYPE_LITERAL,
         AbstractExpression::TYPE_SELECT,
         AbstractExpression::TYPE_VALUE
};

AbstractExpression::AbstractExpression()
{
}

AbstractExpression::ExpressionDataType AbstractExpression::getExpressionData()const
{
   return {};
}

QPair<QString, QString> AbstractExpression::normalizeArgument(const QString &argument, const QString &argumentType)const
{
   if(!AbstractExpression::sm_allowTypes.contains(argumentType)){
      throw ErrorInfo(QString("Argument type should be in array(%1)").arg(AbstractExpression::sm_allowTypes.join(',')));
   }
   return {argument, argumentType};
}

AbstractExpression::~AbstractExpression()
{}

}//sql
}//db
}//corelib
}//sn