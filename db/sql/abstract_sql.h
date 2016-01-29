#ifndef SN_CORELIB_DB_SQL_ABSTRACT_SQL_H
#define SN_CORELIB_DB_SQL_ABSTRACT_SQL_H

#include <QMap>
#include <QVariant>
#include <QStringList>
#include <QString>
#include <QObject>
#include <QSharedPointer>
#include <QVariant>
#include <QList>

#include "global/global.h"
#include "db/engine/parameter_container.h"
#include "db/engine/engine.h"
#include "kernel/errorinfo.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::db::engine::Engine;
using sn::corelib::db::engine::ParameterContainer;
using sn::corelib::ErrorInfo;

class Select;
class TableIdentifier;
class AbstractExpression;

class SN_CORELIB_EXPORT AbstractSql
{
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
   AbstractSql& setSubject(QSharedPointer<AbstractSql> subject);
public:
   virtual QString getSqlString(const Engine &engine);
   //内部使用
   virtual QString getDecoratorClassName()const;
   virtual ~AbstractSql();
protected:
   QString buildSqlString(const Engine &engine, ParameterContainer *parameterContainer = nullptr);
   QString createSqlFromSpecificationAndParameters(const QVariant &specification, const QList<QVariant> &parameters)throw(ErrorInfo);
   AbstractSql& setSpecificationFn(const QString &name, SpecificationFuncPtr fn);
   QString resolveTable(const TableIdentifier &table, const Engine &engine, 
                        ParameterContainer *parameterContainer = nullptr);
   QString resolveTable(const QString &table, const Engine &engine, 
                                     ParameterContainer *parameterContainer);
   QString resolveTable(const QVariant &vtable, const Engine &engine, 
                        ParameterContainer *parameterContainer = nullptr);
   QString processExpression(const QSharedPointer<AbstractExpression> expression, const Engine &engine, 
                             ParameterContainer *parameterContainer = nullptr, 
                             QString namedParameterPrefix = QString());
   QString resolveColumnValue(const QVariant &column, const Engine &engine,
                              ParameterContainer *parameterContainer = nullptr, QString namedParameterPrefix = QString());
protected:
   virtual QString processSubSelect(QSharedPointer<Select> subSelect, const Engine &engine, 
                            ParameterContainer *parameterContainer = nullptr)throw(ErrorInfo);
protected:
   QStringList m_specKeys;
   QMap<QString, QVariant> m_specifications;
   QMap<QString, QVariant> m_processInfo;
   QMap<QString, int> m_instanceParameterIndex;
   QMap<QString, SpecificationFuncPtr> m_specificationFnPtrs;
   QSharedPointer<AbstractSql> m_subject;
};

}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_ABSTRACT_SQL_H
