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

AbstractPlatform::DecoratorPoolType AbstractPlatform::getDecorators()const
{
   return m_decorators;
}

QSharedPointer<AbstractSql> AbstractPlatform::getTypeDecorator(QSharedPointer<AbstractSql> subject)
{
   DecoratorPoolType::const_iterator it = m_decorators.cbegin();
   while(it != m_decorators.cend()){
      QSharedPointer<AbstractSql> decorator = it.value();
      if(it.key() == decorator->getDecoratorClassName()){
         decorator->setSubject(subject);
         return decorator;
      }
      it++;
   }
   return subject;
}

QString AbstractPlatform::getSqlString(Engine&)
{
   return QString();
}

AbstractPlatform::~AbstractPlatform()
{}

}//platform
}//sql
}//db
}//corelib
}//sn