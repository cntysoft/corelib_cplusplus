#include "abstract_table_object.h"
#include "column_object.h"
#include "constraint_object.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace object{

AbstractTableObject::AbstractTableObject(const QString &name)
   : m_name(name)
{
   
}

AbstractTableObject& AbstractTableObject::setName(const QString &name)
{
   m_name = name;
   return *this;
}

const QString& AbstractTableObject::getName()const
{
   return m_name;
}

AbstractTableObject& AbstractTableObject::setColumns(const QList<QSharedPointer<ColumnObject>> &columns)
{
   m_columns = columns;
   return *this;
}

const QList<QSharedPointer<ColumnObject>>& AbstractTableObject::getColumns()const
{
   return m_columns;
}

AbstractTableObject& AbstractTableObject::setConstraints(const QList<QSharedPointer<ConstraintObject>> &constraints)
{
   m_constraints = constraints;
   return *this;
}

const QList<QSharedPointer<ConstraintObject>>& AbstractTableObject::getConstraints()const
{
   return m_constraints;
}

AbstractTableObject::~AbstractTableObject()
{}

TableObject::TableObject(const QString &name)
   : AbstractTableObject(name)
{}

}//object
}//metadata
}//db
}//corelib
}//sn