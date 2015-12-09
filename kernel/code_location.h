#ifndef SN_CORELIB_KERNEL_CODE_LOCATION_H
#define SN_CORELIB_KERNEL_CODE_LOCATION_H

#include <QExplicitlySharedDataPointer>
#include <QDebug>

#include "global/global.h"

QT_BEGIN_NAMESPACE
class QDataStream;
class QString;
QT_END_NAMESPACE

namespace sn
{
namespace corelib 
{

class SN_CORELIB_EXPORT CodeLocation
{
   friend SN_CORELIB_EXPORT bool operator==(const CodeLocation &cl1, const CodeLocation &cl2);
public:
   CodeLocation();
   CodeLocation(const QString &filePath, int line = -1, int column = -1);
   CodeLocation(const CodeLocation &other);
   CodeLocation &operator=(const CodeLocation &other);
   ~CodeLocation();
   
   QString getFilePath() const;
   int getLine() const;
   int getColumn() const;
   
   bool isValid() const;
   QString toString() const;
   
private:
   class CodeLocationPrivate;
   QExplicitlySharedDataPointer<CodeLocationPrivate> d;
   
};

SN_CORELIB_EXPORT bool operator==(const CodeLocation &cl1, const CodeLocation &cl2);
SN_CORELIB_EXPORT bool operator!=(const CodeLocation &cl1, const CodeLocation &cl2);

QDebug operator<<(QDebug debug, const CodeLocation &location);

}//corelib
}//sn

#endif // SN_CORELIB_KERNEL_CODE_LOCATION_H

