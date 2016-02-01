#ifndef SN_CORELIB_DB_METADATA_OBJECT_TRIGGER_OBJECT_H
#define SN_CORELIB_DB_METADATA_OBJECT_TRIGGER_OBJECT_H

#include <QString>
#include <QDateTime>

#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace object{

class SN_CORELIB_EXPORT TriggerObject
{
public:
   const QString& getName()const;
   TriggerObject& setName(const QString &name);
   const QString& getEventManipulation()const;
   TriggerObject& setEventManipulation(const QString &eventManipulation);
   const QString& getEventObjectCatalog()const;
   TriggerObject& setEventObjectCatalog(const QString &eventObjectCatalog);
   const QString& getEventObjectSchema()const;
   TriggerObject& setEventObjectSchema(const QString &eventObjectSchema);
   const QString& getEventObjectTable()const;
   TriggerObject& setEventObjectTable(const QString &eventObjectTable);
   const QString& getActionOrder()const;
   TriggerObject& setActionOrder(const QString &actionOrder);
   const QString& getActionCondition()const;
   TriggerObject& setActionCondition(const QString &actionCondition);
   const QString& getActionStatement()const;
   TriggerObject& setActionStatement(const QString &actionStatement);
   const QString& getActionOrientation()const;
   TriggerObject& setActionOrientation(const QString &actionOrientation);
   const QString& getActionTiming()const;
   TriggerObject& setActionTiming(const QString &actionTiming);
   const QString& getActionReferenceOldTable()const;
   TriggerObject& setActionReferenceOldTable(const QString& actionReferenceOldTable);
   const QString& getActionReferenceNewTable()const;
   TriggerObject& setActionReferenceNewTable(const QString& actionReferenceNewTable);
   const QString& getActionReferenceOldRow()const;
   TriggerObject& setActionReferenceOldRow(const QString& actionReferenceOldRow);
   const QString& getActionReferenceNewRow()const;
   TriggerObject& setActionReferenceNewRow(const QString& actionReferenceNewRow);
   QDateTime getCreated()const;
   TriggerObject& setCreated(const QDateTime &created);
   protected:
   QString m_name;
   QString m_eventManipulation;
   QString m_eventObjectCatalog;
   QString m_eventObjectSchema;
   QString m_eventObjectTable;
   QString m_actionOrder;
   QString m_actionCondition;
   QString m_actionStatement;
   QString m_actionOrientation;
   QString m_actionTiming;
   QString m_actionReferenceOldTable;
   QString m_actionReferenceNewTable;
   QString m_actionReferenceOldRow;
   QString m_actionReferenceNewRow;
   QDateTime m_created;
};

}//object
}//metadata
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_METADATA_OBJECT_TRIGGER_OBJECT_H
