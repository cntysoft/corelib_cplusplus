#include "create_table_decorator.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{
namespace mysql{

QString CreateTableDecorator::getDecoratorClassName()const
{
   return QString("sn::corelib::db::sql::platform::mysql::CreateTableDecorator");
}

}//mysql
}//platform
}//sql
}//db
}//corelib
}//sn
