#include "simple_route_item.h"

namespace sn 
{
namespace corelib 
{

SimpleRouteItem::SimpleRouteItem(const QString& route)
{
   m_matcher = new RouteMatcher(route);
}

SimpleRouteItem::~SimpleRouteItem()
{
   delete m_matcher;
}

}//corelib
}//sn