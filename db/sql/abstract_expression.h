#ifndef SN_CORELIB_DB_SQL_ABSTRACT_EXPRESSION_H
#define SN_CORELIB_DB_SQL_ABSTRACT_EXPRESSION_H

#include <QStringList>
#include <QSharedPointer>
#include <QObject>
#include <QVector>
#include <QVariant>
#include <QString>

#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

class SN_CORELIB_EXPORT AbstractExpression : public QObject
{
   Q_OBJECT
public:
   struct ExpressionData
   {
      bool isLiteral;
      QString specification;
      QStringList values;
      QStringList types;
   };
   using ExpressionDataType = QVector<QVariant>;
public:
   const static QString TYPE_IDENTIFIER;
   const static QString TYPE_VALUE;
   const static QString TYPE_LITERAL;
   const static QString TYPE_SELECT;
public:
   AbstractExpression();
   virtual ExpressionDataType getExpressionData()const;
   virtual ~AbstractExpression();
protected:
   QPair<QVariant, QString> normalizeArgument(const QVariant &argument, const QString &defaultType = AbstractExpression::TYPE_VALUE)const;
protected:
   QStringList m_allowTypes;
};

}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_ABSTRACT_EXPRESSION_H