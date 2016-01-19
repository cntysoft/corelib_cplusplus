#include "abstract_expression.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

AbstractExpression::AbstractExpression()
{
   m_allowTypes.append(AbstractExpression::TYPE_IDENTIFIER);
   m_allowTypes.append(AbstractExpression::TYPE_LITERAL);
   m_allowTypes.append(AbstractExpression::TYPE_SELECT);
   m_allowTypes.append(AbstractExpression::TYPE_VALUE);
}

}//sql
}//db
}//corelib
}//sn