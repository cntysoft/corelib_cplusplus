#include <QString>

#include "abstract_platform.h"
#include <QDebug>

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{

AbstractPlatform& AbstractPlatform::setSubject(QSharedPointer<AbstractSql> subject)
{
   m_subject = subject;
   return *this;
}

AbstractPlatform& AbstractPlatform::setTypeDecorator(const QString &type, QSharedPointer<AbstractSql> decorator)
{
   if(m_decorators.contains(type)){
      m_decorators.remove(type);
   }
   m_decorators[type] = decorator;
   return *this;
}

QMap<QString, QSharedPointer<AbstractSql>>& AbstractPlatform::getDecorators()
{
   return m_decorators;
}

QSharedPointer<AbstractSql> AbstractPlatform::getTypeDecorator(QSharedPointer<AbstractSql> subject)
{
//   QString className(subject->metaObject()->className());
//   DecoratorPoolType::const_iterator it = m_decorators.cbegin();
//   while(it != m_decorators.cend()){
//      if(className == it.key()){
//         return it.value();
//      }
//      it++;
//   }
   return subject;
}

QString AbstractPlatform::getSqlString(Engine &engine)
{
   return QString("asdasdas");
}

}//platform
}//sql
}//db
}//corelib
}//sn