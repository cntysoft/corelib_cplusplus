#include "abstract_platform.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{

AbstractPlatform& AbstractPlatform::setSubject(QSharedPointer<PreparableSqlInterface> subject)
{
   m_prepareSubject = subject;
   return *this;
}

AbstractPlatform& AbstractPlatform::setSubject(QSharedPointer<SqlInterface> subject)
{
   m_normalSubject = subject;
   return *this;
}

AbstractPlatform& AbstractPlatform::setTypeDecorator(const QString &type, PlatformDecoratorInterface *decorator)
{
   if(m_decorators.contains(type)){
      delete m_decorators.take(type);
   }
   m_decorators[type] = decorator;
   return *this;
}

QMap<QString, PlatformDecoratorInterface*>& AbstractPlatform::getDecorators()
{
   return m_decorators;
}

QString AbstractPlatform::getSqlString(Engine &engine)
{
   return QString();
}

}//platform
}//sql
}//db
}//corelib
}//sn