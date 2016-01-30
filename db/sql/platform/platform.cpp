#include <QScopedPointer>

#include "platform.h"
#include "db/sql/platform/platform_types.h"


namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{

using sn::corelib::db::sql::platform::Mysql;

Platform::Platform(Engine &engine)
   : m_engine(engine),
     m_defaultPlatform(Engine::PlatformType::Mysql)
{
   QScopedPointer<Mysql> mysqlPlatform(new Mysql);
   m_decorators.insert(Engine::PlatformType::Mysql, mysqlPlatform->getDecorators());
}
Platform& Platform::setTypeDecorator(const QString &type, QSharedPointer<AbstractSql> decorator, Engine::PlatformType platformType)
{
   if(!m_decorators.contains(platformType)){
      m_decorators.insert(platformType, DecoratorPoolType());
   }
   DecoratorPoolType& decorators = m_decorators[platformType];
   if(decorators.contains(type)){
      decorators.remove(type);
   }
   decorators.insert(type, decorator);
   return *this;
}

Platform& Platform::setTypeDecorator(const QString &type, QSharedPointer<AbstractSql> decorator)
{
   return setTypeDecorator(type, decorator, m_engine.getPlatformType());
}

QSharedPointer<AbstractSql> Platform::getTypeDecorator(QSharedPointer<AbstractSql> subject, Engine::PlatformType platformType)
{
   if(!m_decorators.contains(platformType)){
      return subject;
   }
   DecoratorPoolType& decorators = m_decorators[platformType];
   DecoratorPoolType::const_iterator it = decorators.cbegin();
   while(it != decorators.cend()){
      QSharedPointer<AbstractSql> decorator = it.value();
      if(it.key() == subject->getDecoratorClassName()){
         decorator->setSubject(subject);
         return decorator;
      }
      it++;
   }
   return subject;
}

QString Platform::getSqlString(Engine &engine)
{
   return getTypeDecorator(m_subject, engine.getPlatformType())->getSqlString(engine);
}

AbstractPlatform::DecoratorPoolType Platform::getDecorators()const
{
   Engine::PlatformType ptype = m_engine.getPlatformType();
   if(!m_decorators.contains(ptype)){
      ptype =  Engine::PlatformType::Mysql;
   }
   return m_decorators[ptype];
}

}//platform
}//sql
}//db
}//corelib
}//sn
