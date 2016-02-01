#include "constraint_object.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace object{

ConstraintObject::ConstraintObject(const QString &name, const QString &tableName, const QString &schemaName)
   : m_name(name),
     m_tableName(tableName),
     m_schemaName(schemaName)
{
   
}

ConstraintObject& ConstraintObject::setName(const QString &name)
{
   m_name = name;
   return *this;
}

const QString& ConstraintObject::getName()const
{
   return m_name;
}

ConstraintObject& ConstraintObject::setSchemaName(const QString& schemaName)
{
   m_schemaName = schemaName;
   return *this;
}

const QString& ConstraintObject::getSchemaName()const
{
   return m_schemaName;
}

ConstraintObject& ConstraintObject::setTableName(const QString &tableName)
{
   m_tableName = tableName;
   return *this;
}

const QString& ConstraintObject::getTableName()const
{
   return m_tableName;   
}

ConstraintObject& ConstraintObject::setType(const QString &type)
{
   m_type = type;
   return *this;
}

const QString& ConstraintObject::getType()const
{
   return m_type;
}

bool ConstraintObject::hasColumns()const
{
   return !m_columns.isEmpty();
}
const QStringList& ConstraintObject::getColumns()const
{
   return m_columns;
}
ConstraintObject& ConstraintObject::setColumns(const QStringList &columns)
{
   m_columns = columns;
   return *this;
}

const QString& ConstraintObject::getReferencedTableSchema()const
{
   return m_referencedTableSchema;
}
ConstraintObject& ConstraintObject::setReferencedTableSchema(const QString &referencedTableSchema)
{
   m_referencedTableSchema = referencedTableSchema;
   return *this;
}

const QString& ConstraintObject::getReferencedTableName()const
{
   return m_referencedTableName;
}

ConstraintObject& ConstraintObject::setReferencedTableName(const QString &referencedTableName)
{
   m_referencedTableName = referencedTableName;
   return *this;
}

const QStringList& ConstraintObject::getReferencedColumns()const
{
   return m_referencedColumns;
}
ConstraintObject& ConstraintObject::setReferencedColumns(const QStringList &referencedColumns)
{
   m_referencedColumns = referencedColumns;
   return *this;
}

const QString& ConstraintObject::getMatchOption()const
{
   return m_matchOption;
}

ConstraintObject& ConstraintObject::setMatchOption(const QString &matchOption)
{
   m_matchOption = matchOption;
   return *this;
}

const QString& ConstraintObject::getUpdateRule()const
{
   return m_updateRule;   
}
ConstraintObject& ConstraintObject::setUpdateRule(const QString &updateRule)
{
   m_updateRule = updateRule;
   return *this;
}

const QString& ConstraintObject::getDeleteRule()const
{
   return m_deleteRule;  
}
ConstraintObject& ConstraintObject::setDeleteRule(const QString &deleteRule)
{
   m_deleteRule = deleteRule;
   return *this;   
}
const QString& ConstraintObject::getCheckClause()const
{
   return m_checkClause;
}
ConstraintObject& ConstraintObject::setCheckClause(const QString &checkClause)
{
   m_checkClause = checkClause;
   return *this;
}

bool ConstraintObject::isPrimaryKey()const
{
   return "PRIMARY KEY" == m_type;
}
bool ConstraintObject::isUnique()const
{
   return "UNIQUE" == m_type;  
}

bool ConstraintObject::isForeignKey()const
{
   return "FOREIGN KEY" == m_type;
}

bool ConstraintObject::isCheck()const
{
   return "CHECK" == m_type;
}

}//object
}//metadata
}//db
}//corelib
}//sn