#include <QList>

#include "errorinfo.h"

namespace sn{
namespace corelib{
class ErrorInfo::ErrorInfoPrivate : public QSharedData
{
public:
   ErrorInfoPrivate()
      : internalError(false)
   {}
   QList<ErrorItem> items;
   bool internalError;
   QMap<QString, QVariant> m_extra;
};


ErrorInfo::ErrorInfo(const ErrorInfo &rhs)
   :d(rhs.d)
{
   
}

ErrorInfo::ErrorInfo(const QString &description, int errorCode, const CodeLocation &codeLocation, bool internalError)
   :d(new ErrorInfoPrivate)
{
   append(description, errorCode, codeLocation);
   d->internalError = internalError;
}

ErrorInfo::~ErrorInfo()
{}

ErrorInfo& ErrorInfo::operator=(const ErrorInfo &other)
{
   d = other.d;
   return *this;
}

void ErrorInfo::append(const QString &description, int errorCode, const CodeLocation &codeLocation)
{
   d->items.append(ErrorItem(description, errorCode, codeLocation));
}

void ErrorInfo::prepend(const QString &description, int errorCode, const CodeLocation &codeLocation)
{
   d->items.prepend(ErrorItem(description, errorCode, codeLocation));
}

QList<ErrorItem> ErrorInfo::getItems() const
{
   return d->items;
}

const ErrorItem& ErrorInfo::getFirstErrorItem()const
{
   return d->items.first();
}

ErrorInfo& ErrorInfo::setExtraErrorInfo(const QString &key, const QVariant &value)
{
   d->m_extra.insert(key, value);
   return *this;
}

const QMap<QString, QVariant>& ErrorInfo::getExtraErrorInfos()const
{
   return d->m_extra;
}

void ErrorInfo::clear()
{
   d->items.clear();
}

QString ErrorInfo::toString() const
{
   QStringList lines;
   foreach (const ErrorItem &error, d->items) {
      lines.append(error.toString());
   }
   return lines.join(QLatin1Char('\n'));
}

bool ErrorInfo::isInternalError() const
{
   return d->internalError;
}
}//corelib
}//sn