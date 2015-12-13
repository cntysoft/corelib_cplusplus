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
   RouteMatchResult(bool status, const RouteArgsType& params, int length = 0);
   RouteMatchResult& setMatchedRouteName(const QString& name);
   QString getMatchedRouteName()const;
   RouteMatchResult& setParam(const QString& name, const QString& value);
   QString getParam(const QString& name, QString defaultValue = QString())const;
   RouteArgsType getParams()const;
   int getLength()const;
   bool getStatus()const;
   RouteMatchResult& merge(RouteMatchResult& routeMatchResult);
protected:
   QString m_matchedRouteName;
   bool m_status;
   int m_length;
   RouteArgsType m_params;
};

}//corelib
}//sn
#endif // SN_CORELIB_COMMAND_ROUTE_MATCH_H
