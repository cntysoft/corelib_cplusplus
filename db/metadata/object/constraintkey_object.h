#ifndef SN_CORELIB_DB_METADATA_OBJECT_CONSTRAINTKEY_OBJECT_H
#define SN_CORELIB_DB_METADATA_OBJECT_CONSTRAINTKEY_OBJECT_H

#include <QString>

#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace object{

class SN_CORELIB_EXPORT ConstraintKeyObject
{
public:
   const static QString FK_CASCADE;
   const static QString FK_SET_NULL;
   const static QString FK_NO_ACTION;
   const static QString FK_RESTRICT;
   const static QString FK_SET_DEFAULT;
public:
   ConstraintKeyObject(const QString &column);
   const QString& getColumnName()const;
   ConstraintKeyObject& setColumnName(const QString &columnName);
   int getOrdinalPosition()const;
   ConstraintKeyObject& setOrdinalPosition(int ordinalPosition);
   bool getPositionInUniqueConstraint()const;
   ConstraintKeyObject& setPositionInUniqueConstraint(bool flag);
   const QString& getReferencedTableSchema()const;
   ConstraintKeyObject& setReferencedTableSchema(const QString &referencedTableSchema);
   const QString& getReferencedTableName()const;
   ConstraintKeyObject& setReferencedTableName(const QString &referencedTableName);
   const QString& getReferencedColumnName()const;
   ConstraintKeyObject& setReferencedColumnName(const QString &referencedColumnName);
   const QString& getForeignKeyUpdateRule()const;
   ConstraintKeyObject& setForeignKeyUpdateRule(const QString &foreignKeyUpdateRule);
   const QString& getForeignKeyDeleteRule()const;
   ConstraintKeyObject& setForeignKeyDeleteRule(const QString &foreignKeyDeleteRule);
protected:
   QString m_columnName;
   int m_ordinalPosition;
   bool m_positionInUniqueConstraint;
   QString m_referencedTableSchema;
   QString m_referencedTableName;
   QString m_referencedColumnName;
   QString m_foreignKeyUpdateRule;
   QString m_foreignKeyDeleteRule;
};

}//object
}//metadata
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_METADATA_OBJECT_CONSTRAINTKEY_OBJECT_H
