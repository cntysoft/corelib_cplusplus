#ifndef SN_CORELIB_DB_METADATA_OBJECT_ABSTRACT_TABLE_OBJECT_H
#define SN_CORELIB_DB_METADATA_OBJECT_ABSTRACT_TABLE_OBJECT_H

#include <QString>
#include <QSharedPointer>

#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace object{

class ConstraintObject;
class ColumnObject;

class SN_CORELIB_EXPORT AbstractTableObject
{
public:
   AbstractTableObject(const QString &name);
   AbstractTableObject& setName(const QString &name);
   const QString& getName()const;
   AbstractTableObject& setColumns(const QList<QSharedPointer<ColumnObject>> &columns);
   const QList<QSharedPointer<ColumnObject>>& getColumns()const;
   AbstractTableObject& setConstraints(const QList<QSharedPointer<ConstraintObject>> &constraints);
   const QList<QSharedPointer<ConstraintObject>>& getConstraints()const;
   virtual ~AbstractTableObject();
protected:
   QString m_name;
   QString m_type;
   QList<QSharedPointer<ColumnObject>> m_columns;
   QList<QSharedPointer<ConstraintObject>> m_constraints;
};

class SN_CORELIB_EXPORT TableObject : public AbstractTableObject
{
public:
   TableObject(const QString &name);
};

}//object
}//metadata
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_METADATA_OBJECT_ABSTRACT_TABLE_OBJECT_H
