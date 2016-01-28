#include "abstract_preparable_sql.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

StatementContainerInterface& AbstractPreparableSql::prepareStatement(const Engine&, const StatementContainerInterface&)
{
   
}

AbstractPreparableSql::~AbstractPreparableSql()
{}

}//sql
}//db
}//corelib
}//sn