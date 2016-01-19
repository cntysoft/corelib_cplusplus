#include "interface.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

const QString ExpressionInterface::TYPE_IDENTIFIER = "identifier";
const QString ExpressionInterface::TYPE_VALUE = "value";
const QString ExpressionInterface::TYPE_LITERAL = "literal";
const QString ExpressionInterface::TYPE_SELECT = "select";

ExpressionInterface::~ExpressionInterface()
{}

}//sql
}//db
}//corelib
}//sn