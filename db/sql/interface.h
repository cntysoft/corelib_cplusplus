#ifndef SN_CORELIB_DB_SQL_INTERFACE_H
#define SN_CORELIB_DB_SQL_INTERFACE_H

#include <QSharedPointer>

#include "global/global.h"
#include "db/engine/engine.h"
#include "db/engine/interface.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{
class Platform;
}//platform
using sn::corelib::db::engine::Engine;
using sn::corelib::db::engine::StatementContainerInterface;

class SN_CORELIB_EXPORT SqlInterface
{
public:
   QString getSqlString();
};

class SN_CORELIB_EXPORT PreparableSqlInterface
{
public:
   virtual StatementContainerInterface& prepareStatement(const Engine &engine, const StatementContainerInterface &statementContainer) = 0;
};

class SN_CORELIB_EXPORT PlatformDecoratorInterface
{
public:
   virtual platform::Platform& setSubject(const PreparableSqlInterface& subject) = 0;
   virtual platform::Platform& setSubject(const SqlInterface &subject) = 0;
};


class SN_CORELIB_EXPORT ExpressionInterface
{
public:
   struct ExpressionData
   {
      bool isLiteral;
      QString specification;
      QStringList values;
      QStringList types;
   };
public:
   const static QString TYPE_IDENTIFIER;
   const static QString TYPE_VALUE;
   const static QString TYPE_LITERAL;
   const static QString TYPE_SELECT;
public:
   QSharedPointer<ExpressionData> getExpressionData();
   virtual ~ExpressionInterface();
};

}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_INTERFACE_H
