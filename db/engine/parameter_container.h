#ifndef SN_CORELIB_DB_ENGINE_PARAMETER_CONTAINER_H 
#define SN_CORELIB_DB_ENGINE_PARAMETER_CONTAINER_H

#include <QString>
#include <QMap>
#include <QVariant>
#include <QStringList>
#include <QList>

#include "kernel/errorinfo.h"
#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{
namespace engine{

using sn::corelib::ErrorInfo;

class SN_CORELIB_EXPORT ParameterContainer
{
public:
   const static QString TYPE_AUTO;
   const static QString TYPE_NULL;
   const static QString TYPE_DOUBLE;
   const static QString TYPE_INTEGER;
   const static QString TYPE_BINARY;
   const static QString TYPE_STRING;
   const static QString TYPE_LOB;
public:
   ParameterContainer(const QMap<QString, QVariant> data = QMap<QString, QVariant>());
   
   ParameterContainer& offsetSet(const QString &name, const QVariant &value, const QString &errata = QString(), int maxLength = -1);
   const QVariant& offsetGet(const QString &name)throw(ErrorInfo);
   ParameterContainer& offsetUnset(const QString &name);
   ParameterContainer& offsetUnset(int position);
   bool offsetExists(const QString &name);
   
   ParameterContainer& offsetSetErrata(const QString &name, const QString &errata);
   ParameterContainer& offsetSetErrata(int postion, const QString &errata);
   const QString& offsetGetErrata(const QString &name) throw(ErrorInfo);
   const QString& offsetGetErrata(int position)throw(ErrorInfo);
   bool offsetHasErrata(const QString &name);
   bool offsetHasErrata(int position);
   ParameterContainer& offsetUnsetErrata(const QString &name);
   ParameterContainer& offsetUnsetErrata(int position);
   
   ParameterContainer& offsetSetMaxLength(const QString &name, int MaxLength);
   ParameterContainer& offsetSetMaxLength(int postion, int MaxLength);
   int offsetGetMaxLength(const QString &name) throw(ErrorInfo);
   int offsetGetMaxLength(int position)throw(ErrorInfo);
   bool offsetHasMaxLength(const QString &name);
   bool offsetHasMaxLength(int position);
   ParameterContainer& offsetUnsetMaxLength(const QString &name);
   ParameterContainer& offsetUnsetMaxLength(int position);
   
   const QMap<QString, QVariant>& getNamedDataMap();
   QList<QVariant> getPositionalDataList();
   int count()const;
   ParameterContainer& merge(const ParameterContainer &params);
   
protected:
   QMap<QString, QVariant> m_data;
   QMap<QString, QString> m_errata;
   QMap<QString, int> m_maxLength;
   QStringList m_positions;
};

}//engine
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_ENGINE_PARAMETER_CONTAINER_H