#include "constraintkey_object.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace object{

const QString ConstraintKeyObject::FK_CASCADE = "CASCADE";
const QString ConstraintKeyObject::FK_SET_NULL = "SET NULL";
const QString ConstraintKeyObject::FK_NO_ACTION = "NO ACTION";
const QString ConstraintKeyObject::FK_RESTRICT = "RESTRICT";
const QString ConstraintKeyObject::FK_SET_DEFAULT = "SET DEFAULT";

ConstraintKeyObject::ConstraintKeyObject(const QString &column)
   : m_columnName(column)
{}

const QString& ConstraintKeyObject::getColumnName()const
{
   return m_columnName;   
}

ConstraintKeyObject& ConstraintKeyObject::setColumnName(const QString &columnName)
{
   m_columnName = columnName;
   return *this;
}

int ConstraintKeyObject::getOrdinalPosition()const
{
   return m_ordinalPosition;
}

ConstraintKeyObject& ConstraintKeyObject::setOrdinalPosition(int ordinalPosition)
{
   m_ordinalPosition = ordinalPosition;
   return *this;
}

bool ConstraintKeyObject::getPositionInUniqueConstraint()const
{
   return m_positionInUniqueConstraint;   
}

ConstraintKeyObject& ConstraintKeyObject::setPositionInUniqueConstraint(bool flag)
{
   m_positionInUniqueConstraint = flag;
   return *this;
}

const QString& ConstraintKeyObject::ConstraintKeyObject::getReferencedTableSchema()const
{
   return m_referencedTableSchema;
}

ConstraintKeyObject& ConstraintKeyObject::setReferencedTableSchema(const QString &referencedTableSchema)
{
   m_referencedTableSchema = referencedTableSchema;
   return *this;
}

const QString& ConstraintKeyObject::getReferencedTableName()const
{
   return m_referencedTableName;
}

ConstraintKeyObject& ConstraintKeyObject::setReferencedTableName(const QString &referencedTableName)
{
   m_referencedTableName = referencedTableName;
   return *this;
}

const QString& ConstraintKeyObject::getReferencedColumnName()const
{
   return m_referencedColumnName;
}
ConstraintKeyObject& ConstraintKeyObject::setReferencedColumnName(const QString &referencedColumnName)
{
   m_referencedColumnName = referencedColumnName;
   return *this;
}

const QString& ConstraintKeyObject::getForeignKeyUpdateRule()const
{
   return m_foreignKeyUpdateRule;
}

ConstraintKeyObject& ConstraintKeyObject::setForeignKeyUpdateRule(const QString &foreignKeyUpdateRule)
{
   m_foreignKeyUpdateRule = foreignKeyUpdateRule;
   return *this;
}

const QString& ConstraintKeyObject::getForeignKeyDeleteRule()const
{
   return m_foreignKeyDeleteRule;
}

ConstraintKeyObject& ConstraintKeyObject::setForeignKeyDeleteRule(const QString &foreignKeyDeleteRule)
{
   m_foreignKeyDeleteRule = foreignKeyDeleteRule;
   return *this;
}

}//object
}//metadata
}//db
}//corelib
}//sn