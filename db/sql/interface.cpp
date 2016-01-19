#include "interface.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

const static QString ExpressionInterface::TYPE_IDENTIFIER = "identifier";
const static QString ExpressionInterface::TYPE_VALUE = "value";
const static QString ExpressionInterface::TYPE_LITERAL = "literal";
const static QString ExpressionInterface::TYPE_SELECT = "select";

ExpressionInterface::~ExpressionInterface()
{}

}//sql
}//db
}//corelib
}//sn