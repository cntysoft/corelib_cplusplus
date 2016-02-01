#include "trigger_object.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace object{

const QString& TriggerObject::getName()const
{
   return m_name;
}
TriggerObject& TriggerObject::TriggerObject::setName(const QString &name)
{
   m_name = name;
   return *this;
}

const QString& TriggerObject::getEventManipulation()const
{
   return m_eventManipulation;
}
TriggerObject& TriggerObject::setEventManipulation(const QString &eventManipulation)
{
   m_eventManipulation = eventManipulation;
   return *this;
}

const QString& TriggerObject::getEventObjectCatalog()const
{
   return m_eventObjectCatalog;
}

TriggerObject& TriggerObject::setEventObjectCatalog(const QString &eventObjectCatalog)
{
   m_eventObjectCatalog = eventObjectCatalog;
   return *this;
}

const QString& TriggerObject::getEventObjectSchema()const
{
   return m_eventObjectSchema;
}

TriggerObject& TriggerObject::setEventObjectSchema(const QString &eventObjectSchema)
{
   m_eventObjectSchema = eventObjectSchema;
   return *this;
}

const QString& TriggerObject::getEventObjectTable()const
{
   return m_eventObjectTable;
}

TriggerObject& TriggerObject::setEventObjectTable(const QString &eventObjectTable)
{
   m_eventObjectTable = eventObjectTable;
   return *this;
}

const QString& TriggerObject::getActionOrder()const
{
   return m_actionOrder;
}

TriggerObject& TriggerObject::setActionOrder(const QString &actionOrder)
{
   m_actionOrder = actionOrder;
   return *this;
}

const QString& TriggerObject::getActionCondition()const
{
   return m_actionCondition;
}

TriggerObject& TriggerObject::setActionCondition(const QString &actionCondition)
{
   m_actionCondition = actionCondition;
   return *this;
}

const QString& TriggerObject::getActionStatement()const
{
   return m_actionStatement;
}
TriggerObject& TriggerObject::setActionStatement(const QString &actionStatement)
{
   m_actionStatement = actionStatement;
   return *this;
}

const QString& TriggerObject::getActionOrientation()const
{
   return m_actionOrientation;
}
TriggerObject& TriggerObject::setActionOrientation(const QString &actionOrientation)
{
   m_actionOrientation = actionOrientation;
   return *this;
}

const QString& TriggerObject::getActionTiming()const
{
   return m_actionTiming;
}

TriggerObject& TriggerObject::setActionTiming(const QString &actionTiming)
{
   m_actionTiming = actionTiming;
   return *this;
}

const QString& TriggerObject::getActionReferenceOldTable()const
{
   return m_actionReferenceOldTable;   
}

TriggerObject& TriggerObject::setActionReferenceOldTable(const QString& actionReferenceOldTable)
{
   m_actionReferenceOldTable = actionReferenceOldTable;
   return *this;
}

const QString& TriggerObject::getActionReferenceNewTable()const
{
   return m_actionReferenceNewTable;
}
TriggerObject& TriggerObject::setActionReferenceNewTable(const QString& actionReferenceNewTable)
{
   m_actionReferenceNewTable = actionReferenceNewTable;
   return *this;
}

const QString& TriggerObject::getActionReferenceOldRow()const
{
   return m_actionReferenceOldRow;
}

TriggerObject& TriggerObject::setActionReferenceOldRow(const QString& actionReferenceOldRow)
{
   m_actionReferenceOldRow = actionReferenceOldRow;
   return *this;
}

const QString& TriggerObject::getActionReferenceNewRow()const
{
   return m_actionReferenceNewRow;
}

TriggerObject& TriggerObject::setActionReferenceNewRow(const QString& actionReferenceNewRow)
{
   m_actionReferenceNewRow = actionReferenceNewRow;
   return *this;
}

QDateTime TriggerObject::getCreated()const
{
   return m_created;   
}

TriggerObject& TriggerObject::setCreated(const QDateTime &created)
{
   m_created = created;
   return *this;
}

}//object
}//metadata
}//db
}//corelib
}//sn