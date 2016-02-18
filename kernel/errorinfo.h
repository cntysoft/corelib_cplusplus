#ifndef SN_CORELIB_KERNEL_CODE_ERRORINFO_H
#define SN_CORELIB_KERNEL_CODE_ERRORINFO_H

#include <QSharedDataPointer>

#include "global/global.h"
#include "code_location.h"
#include "erroritem.h"
QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

namespace sn{
namespace corelib{
class SN_CORELIB_EXPORT ErrorInfo
{
public:
   ErrorInfo(const ErrorInfo &rhs);
   ErrorInfo(const QString &description = QString(), int errorCode = -1, const CodeLocation &codeLocation = CodeLocation(), bool internalError = false);
   ErrorInfo &operator=(const ErrorInfo &other);
   ~ErrorInfo();
   void append(const QString &description, int errorCode, const CodeLocation &codeLocation = CodeLocation());
   void prepend(const QString &description, int errorCode, const CodeLocation &codeLocation = CodeLocation());
   QList<ErrorItem> getItems() const;
   const ErrorItem& getFirstErrorItem()const;
   bool hasError() const
   {
      return !getItems().isEmpty();
   }
   void clear();
   QString toString() const;
   bool isInternalError() const;
   ErrorInfo& setExtraErrorInfo(const QString &key, const QVariant &value);
   const QMap<QString, QVariant>& getExtraErrorInfos()const;
private:
   class ErrorInfoPrivate;
   QSharedDataPointer<ErrorInfoPrivate> d;
};
}//corelib
}//sn

Q_DECLARE_METATYPE(sn::corelib::ErrorInfo)
#endif // SN_CORELIB_KERNEL_CODE_ERRORINFO_H