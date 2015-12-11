#ifndef SN_CORELIB_COMMAND_ROUTE_MATCH_H
#define SN_CORELIB_COMMAND_ROUTE_MATCH_H

#include <QMap>
#include <QString>

#include "global/global.h"

namespace sn 
{
namespace corelib 
{

class SN_CORELIB_EXPORT RouteMatchResult
{
public:
   using RouteArgsType = QMap<QString, QString>;
public:
   RouteMatchResult(const RouteArgsType& params);
   RouteMatchResult& setMatchedRouteName(const QString& name);
   QString& getMatchedRouteName();
   RouteMatchResult& setParam(const QString& name, const QString& value);
   QString& getParam(const QString& name, const QString& defaultValue = QString());
   RouteArgsType& getParams();
protected:
   QString m_matchedRouteName;
   RouteArgsType m_params;
};

}//corelib
}//sn
#endif // SN_CORELIB_COMMAND_ROUTE_MATCH_H
