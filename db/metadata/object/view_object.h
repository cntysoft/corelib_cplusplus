#ifndef SN_CORELIB_DB_METADATA_OBJECT_VIEW_OBJECT_H
#define SN_CORELIB_DB_METADATA_OBJECT_VIEW_OBJECT_H

#include "global/global.h"
#include "abstract_table_object.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace object{

class SN_CORELIB_EXPORT ViewObject : public AbstractTableObject
{
public:
   ViewObject(const QString &name);
   ViewObject& setViewDefinition(const QString &viewDefinition);
   const QString& getViewDefinition()const;
   const QString& getCheckOption()const;
   ViewObject& setCheckOption(const QString &checkOption);
   ViewObject& setIsUpdatable(bool flag);
   bool isUpdatable()const;
protected:
   QString m_viewDefinition;
   QString m_checkOption;
   bool m_isUpdatable;
};

}//object
}//metadata
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_METADATA_OBJECT_VIEW_OBJECT_H
