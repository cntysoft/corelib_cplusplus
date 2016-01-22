#include <QString>
#include <QStringList>

#include "kernel/errorinfo.h"
#include "expression.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::ErrorInfo;

const QChar Expression::PLACEHOLDER = '?';

Expression::Expression(const QString &expression, const QStringList &parameters)
   : m_expression(expression),
     m_parameters(parameters)
{   
}

Expression& Expression::setExpression(const QString &expression)
{
   m_expression = expression;
   return *this;
}

const QString& Expression::getExpression()const
{
   return m_expression;
}

Expression& Expression::setParameters(const QStringList &parameters) 
{
   m_parameters = parameters;
   return *this;
}

const QStringList& Expression::getParameters()const
{
   return m_parameters;
}

AbstractExpression::ExpressionDataType Expression::getExpressionData()const
{
   int parametersCount = m_parameters.size();
   QString targetExpr(m_expression);
   targetExpr.replace("%", "%%");
   if(parametersCount == 0){
      return {
         targetExpr.replace(Expression::PLACEHOLDER, "")
      };
   }
   QList<int> placeHolders;
   int exprLength = targetExpr.size();
   for(int i = 0; i < exprLength; i++){
      if(targetExpr.at(i) == Expression::PLACEHOLDER){
         placeHolders << i;
      }
   }
   if(placeHolders.size() != parametersCount){
      throw ErrorInfo("The number of replacements in the expression does not match the number of parameters");
   }
   QList<int>::const_iterator it = placeHolders.cbegin();
   int serial = 1;
   while(it != placeHolders.cend()){
      targetExpr.replace((*it+serial - 1), 1, QString("%%1").arg(serial));
      it++;
      serial++;
   }
   QList<QVariant> values;
   QList<QString> types;
   for(int i = 0; i < parametersCount; i++){
      QPair<QVariant, QString> normalizedArgument = normalizeArgument(m_parameters.at(i), AbstractExpression::TYPE_VALUE);
      values << normalizedArgument.first;
      types << normalizedArgument.second;
   }
   return {
      QVariant(targetExpr),
      QVariant(values),
      QVariant(types)
   };
}

}//sql
}//db
}//corelib
}//sn