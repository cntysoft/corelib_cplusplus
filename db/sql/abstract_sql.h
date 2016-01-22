#ifndef SN_CORELIB_DB_SQL_ABSTRACT_SQL_H
#define SN_CORELIB_DB_SQL_ABSTRACT_SQL_H

#include <QMap>
#include <QVariant>
#include <QStringList>
#include <QString>
#include <QObject>
#include <QSharedPointer>

#include "global/global.h"
#include "db/engine/parameter_container.h"
#include "db/engine/engine.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::db::engine::Engine;
using sn::corelib::db::engine::ParameterContainer;

class TableIdentifier;
class AbstractExpression;

class SN_CORELIB_EXPORT AbstractSql : public QObject
{
   Q_OBJECT
public:
   enum class ProcessResultType
   {
      String,
      Array
   };
   struct ProcessResult
   {
      bool isNull;
      ProcessResultType type;
      QVariant value;
      QVariant& getValue()
      {
         return value;
      }
   };
   using ProcessResultPointerType = QSharedPointer<ProcessResult>;
   using SpecificationFuncPtr = ProcessResultPointerType (*)(AbstractSql *self, const Engine &engine, ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, QMap<QString, ProcessResultPointerType> &parameters);
public:
   virtual QString getSqlString(const Engine &engine);
protected:
   QString buildSqlString(const Engine &engine, ParameterContainer *parameterContainer = nullptr);
   QString createSqlFromSpecificationAndParameters(const QVariant &specification, const QMap<QString, QVariant> &parameters);
   AbstractSql& setSpecificationFn(const QString &name, SpecificationFuncPtr fn);
   QString resolveTable(const TableIdentifier &table, const Engine &engine, ParameterContainer *parameterContainer = nullptr);
   QString processExpression(const QSharedPointer<AbstractExpression> expression, const Engine &engine, 
                             ParameterContainer *parameterContainer = nullptr, 
                             QString namedParameterPrefix = QString());

protected:
   QMap<QString, QVariant> m_specifications;
   QMap<QString, QVariant> m_processInfo;
   QMap<QString, int> m_instanceParameterIndex;
   QMap<QString, SpecificationFuncPtr> m_specificationFnPtrs;
};

}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_ABSTRACT_SQL_H
