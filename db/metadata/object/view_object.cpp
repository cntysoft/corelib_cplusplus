#include "view_object.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace object{

ViewObject::ViewObject(const QString &name)
   : AbstractTableObject(name)
{
}

ViewObject& ViewObject::setViewDefinition(const QString &viewDefinition)
{
   m_viewDefinition = viewDefinition;
   return *this;
}

const QString& ViewObject::getViewDefinition()const
{
   return m_viewDefinition;
}
const QString& ViewObject::getCheckOption()const
{
   return m_checkOption;
}
ViewObject& ViewObject::setCheckOption(const QString &checkOption)
{
   m_checkOption = checkOption;
   return *this;
}

ViewObject& ViewObject::setIsUpdatable(bool flag)
{
   m_isUpdatable = flag;
   return *this;
}

bool ViewObject::isUpdatable()const
{
   return m_isUpdatable;
}

}//object
}//metadata
}//db
}//corelib
}//sn