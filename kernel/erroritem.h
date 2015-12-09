#ifndef SN_CORELIB_KERNEL_CODE_ERRORITEM_H
#define SN_CORELIB_KERNEL_CODE_ERRORITEM_H

#include <QExplicitlySharedDataPointer>

#include "global/global.h"
//#include "code_location.h"
QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

namespace sn 
{
namespace corelib 
{
class CodeLocation;
class ErrorItem
{
public:
   ErrorItem();
   ErrorItem(const QString &description, const CodeLocation &codeLocation);
   ErrorItem(const ErrorItem &rhs);
   ErrorItem& operator=(const ErrorItem &rhs);
   ~ErrorItem();
   
   QString getDescription() const;
   CodeLocation getCodeLocation()const;
   QString toString() const;
   
private:
   class ErrorItemPrivate;
   QExplicitlySharedDataPointer<ErrorItemPrivate> d;
};
}//corelib
}//sn

#endif // SN_CORELIB_KERNEL_CODE_ERRORITEM_H

