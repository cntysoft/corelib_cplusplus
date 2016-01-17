#ifndef SN_CORELIB_DB_ADAPTER_PLATFORM_PLATFORM_INTERFACE_H
#define SN_CORELIB_DB_ADAPTER_PLATFORM_PLATFORM_INTERFACE_H

#include <QString>

#include "kernel/errorinfo.h"
#include "global/global.h"

QT_BEGIN_INCLUDE_NAMESPACE
class QStringList;
QT_END_INCLUDE_NAMESPACE

namespace sn{
namespace corelib{
namespace db{

class SN_CORELIB_EXPORT PlatformInterface
{
public:
   virtual QString getName() = 0;
   virtual QString getQuoteIdentifierSymbol() = 0;
   virtual QString quoteIdentifier(const QString &identifier) = 0;
   virtual QString quoteIdentifierChain(const QString &identifierChain) = 0;
   virtual QString quoteIdentifierChain(const QStringList &identifierChain) = 0;
   virtual QString getQuoteValueSymbol() = 0;
   virtual QString quoteValue(const QString &value) = 0;
   virtual QString quoteTrustedValue(const QString &value) = 0;
   virtual QString quoteValueList(const QStringList &valueList) = 0;
   virtual QString getIdentifierSeparator() = 0;
   virtual QString quoteIdentifierInFragment(const QString &identifier, const QStringList additionalSafeWords = QStringList()) = 0;
};

}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_ADAPTER_PLATFORM_PLATFORM_INTERFACE_H
