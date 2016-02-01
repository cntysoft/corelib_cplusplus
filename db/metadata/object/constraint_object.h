#ifndef SN_CORELIB_DB_METADATA_OBJECT_CONSTRAINT_OBJECT_H
#define SN_CORELIB_DB_METADATA_OBJECT_CONSTRAINT_OBJECT_H 

#include <QString>
#include <QStringList>

#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace object{

class SN_CORELIB_EXPORT ConstraintObject
{
public:
   ConstraintObject(const QString &name, const QString &tableName, const QString &schemaName = QString());
   ConstraintObject& setName(const QString &name);
   const QString& getName()const;
   ConstraintObject& setSchemaName(const QString& schemaName);
   const QString& getSchemaName()const;
   ConstraintObject& setTableName(const QString &tableName);
   const QString& getTableName()const;
   ConstraintObject& setType(const QString &type);
   const QString& getType()const;
   bool hasColumns()const;
   const QStringList& getColumns()const;
   ConstraintObject& setColumns(const QStringList &columns);
   const QString& getReferencedTableSchema()const;
   ConstraintObject& setReferencedTableSchema(const QString &referencedTableSchema);
   const QString& getReferencedTableName()const;
   ConstraintObject& setReferencedTableName(const QString &referencedTableName);
   const QStringList& getReferencedColumns()const;
   ConstraintObject& setReferencedColumns(const QStringList &referencedColumns);
   const QString& getMatchOption()const;
   ConstraintObject& setMatchOption(const QString &matchOption);
   const QString& getUpdateRule()const;
   ConstraintObject& setUpdateRule(const QString &updateRule);
   const QString& getDeleteRule()const;
   ConstraintObject& setDeleteRule(const QString &deleteRule);
   const QString& getCheckClause()const;
   ConstraintObject& setCheckClause(const QString &checkClause);
   bool isPrimaryKey()const;
   bool isUnique()const;
   bool isForeignKey()const;
   bool isCheck()const;
protected:
   QString m_name;
   QString m_tableName;
   QString m_schemaName;
   QString m_type;
   QStringList m_columns;
   QString m_referencedTableSchema;
   QString m_referencedTableName;
   QStringList m_referencedColumns;
   QString m_matchOption;
   QString m_updateRule;
   QString m_deleteRule;
   QString m_checkClause;
};

}//object
}//metadata
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_METADATA_OBJECT_CONSTRAINT_OBJECT_H
