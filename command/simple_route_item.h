#ifndef SN_CORELIB_COMMAND_ROUTER_H
#define SN_CORELIB_COMMAND_ROUTER_H

#include "global/global.h"
#include "route_match_result.h"

namespace sn
{
namespace corelib
{

class RouteMatcher;

class SN_CORELIB_EXPORT SimpleRouteItem
{
public:
   SimpleRouteItem(const QString& route);
   RouteMatchResult match(QStringList cmdArgs);
   ~SimpleRouteItem();
protected:
   RouteMatcher* m_matcher;
};

}//corelib
}//sn
#endif // SN_CORELIB_COMMAND_ROUTER_H
