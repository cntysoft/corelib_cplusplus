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
public:
   const static QChar PLACEHOLDER;
public:
   
protected:
   QString m_expression;
   QStringList m_parameters;
   QStringList m_types;
};

}//sql
}//db
}//corelib
}//sn


#endif // SN_CORELIB_DB_SQL_EXPRESSION_H
