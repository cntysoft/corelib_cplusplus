#include "platform.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{

Platform::Platform(Engine &engine)
   : m_engine(engine)
{
   
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
//   QString className(subject->metaObject()->className());
//   while(it != decorators.cend()){
//      if(className == it.key()){
//         return it.value();
//      }
//      it++;
//   }
   return subject;
}

QString Platform::getSqlString(Engine &engine)
{
   return getTypeDecorator(m_subject, engine.getPlatformType())->getSqlString(engine);
}

}//platform
}//sql
}//db
}//corelib
}//sn
