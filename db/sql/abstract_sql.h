#ifndef SN_CORELIB_DB_SQL_ABSTRACT_SQL_H
#define SN_CORELIB_DB_SQL_ABSTRACT_SQL_H

#include <QMap>
#include <QVariant>
#include <QStringList>
#include <QString>
#include <QObject>

#include "global/global.h"
#include "interface.h"
#include "db/engine/parameter_container.h"
#include "db/engine/engine.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::db::engine::Engine;
using sn::corelib::db::engine::ParameterContainer;

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
      ProcessResultType type;
      QString stringValue;
      QMap<QString, QString> arrayValue;
      QMap<QString, QString>& getArrayValue()
      {
         return arrayValue;
      }
      QString& getStringValue()
      {
         return stringValue;
      }
   };
   using SpecificationFuncPtr = ProcessResult (*)(const Engine &engine, const ParameterContainer &parameterContainer, QMap<QString, QString> &sqls, QMap<QString, ProcessResult> &parameters);
public:
   virtual QString getSqlString(const Engine &engine);
protected:
   QString buildSqlString(const Engine &engine, const ParameterContainer &parameterContainer);
   QString createSqlFromSpecificationAndParameters(const QString &specification, QMap<QString, ProcessResult>& parameters);
   //QString createSqlFromSpecificationAndParameters(const QString &specification, QMap<QString, ProcessResult>& parameters);
   AbstractSql& setSpecificationFn(const QString &name, SpecificationFuncPtr fn);
protected:
   QMap<QString, QString> m_specifications;
   QMap<QString, QVariant> m_processInfo;
   QMap<QString, QVariant> m_instanceParameterIndex;
   QMap<QString, SpecificationFuncPtr> m_specificationFnPtrs;
};

}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_ABSTRACT_SQL_H
