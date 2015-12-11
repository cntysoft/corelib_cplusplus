#ifndef SN_CORELIB_COMMAND_ROUTER_H
#define SN_CORELIB_COMMAND_ROUTER_H

#include "global/global.h"
#include "route_matcher.h"

namespace sn
{
namespace corelib
{

class SN_CORELIB_EXPORT SimpleRouteItem
{
public:
   SimpleRouteItem(const QString& route);
   ~SimpleRouteItem();
protected:
   RouteMatcher* m_matcher;
};

}//corelib
}//sn
#endif // SN_CORELIB_COMMAND_ROUTER_H
