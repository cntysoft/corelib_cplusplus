#ifndef SN_CORELIB_DB_SQL_ABSTRACT_EXPRESSION_H
#define SN_CORELIB_DB_SQL_ABSTRACT_EXPRESSION_H

#include <QStringList>

#include "global/global.h"
#include "interface.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

class SN_CORELIB_EXPORT AbstractExpression : public ExpressionInterface
{
protected:
   buildNormalizedArgument();
protected:
   QStringList m_allowTypes;
};

}//sql
}//db
}//corelib
}//sn


#endif // SN_CORELIB_DB_SQL_ABSTRACT_EXPRESSION_H
