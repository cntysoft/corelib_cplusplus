#include "erroritem.h"
#include "code_location.h"
#include <QSharedData>

namespace sn 
{
namespace corelib
{
class ErrorItem::ErrorItemPrivate : public QSharedData
{
public:
   QString description;
   int errorCode;
   CodeLocation codeLocation;
};

ErrorItem::ErrorItem()
   :d(new ErrorItemPrivate)
{
}

ErrorItem::~ErrorItem()
{}

ErrorItem::ErrorItem(const QString &description, int errorCode, const CodeLocation &codeLocation)
   :d(new ErrorItemPrivate)
{
   d->description = description;
   d->codeLocation = codeLocation;
   d->errorCode = errorCode;
}

ErrorItem::ErrorItem(const ErrorItem &rhs)
   :d(rhs.d)
{
}

ErrorItem& ErrorItem::operator=(const ErrorItem &rhs)
{
   d = rhs.d;
   return *this;
}

QString ErrorItem::getDescription() const
{
   return d->description;
}

int ErrorItem::getErrorCode()const
{
   return d->errorCode;
}

CodeLocation ErrorItem::getCodeLocation() const
{
   return d->codeLocation;
}

QString ErrorItem::toString() const
{
   QString str = getCodeLocation().toString();
   if(!str.isEmpty()){
      str += QLatin1Char(' ');
   }
   str += getDescription();
   if(getErrorCode() != -1){
      str += QString(" code : %1").arg(getErrorCode());
   }
   return str;
}

}//corelib
}//sn