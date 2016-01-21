#ifndef SN_CORELIB_DB_SQL_LITERAL_H
#define SN_CORELIB_DB_SQL_LITERAL_H

#include <QString>

#include "abstract_expression.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

class SN_CORELIB_EXPORT Literal : public AbstractExpression
{
public:
   Literal(const QString &literal);
   Literal& setLiteral(const QString &literal);
   const QString& getLiteral()const;
public:
   virtual ExpressionDataType getExpressionData()const;
protected:
   QString m_literal;
   
};

}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_LITERAL_H
