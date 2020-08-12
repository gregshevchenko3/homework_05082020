#include "shapepropmodel.h"

#include <QDebug>

ShapePropModel::ShapePropModel(QList<QList<QString>> table, QObject *parent)
    : QAbstractTableModel(parent), m_table(table)
{
}

QVariant ShapePropModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Orientation::Horizontal && role == Qt::DisplayRole)
    {
        qDebug() << "headerData";
        switch (section) {
        case 0:
            return QVariant("Property");
        case 1:
            return QVariant("Value");
        }
    }
    return QVariant();
}

int ShapePropModel::rowCount(const QModelIndex & /*parent*/) const
{
   return m_table.size();
}

int ShapePropModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}

QVariant ShapePropModel::data(const QModelIndex &index, int role) const
{
    qDebug() << "data";
    QVariant result;
    if (role == Qt::DisplayRole){
        if(index.row() < m_table.size())
        {
            QList<QString> row = m_table[index.row()];
            if(index.column() < row.size())
                result = QVariant(row[index.column()]);
        }
    }
    return result;
}

bool ShapePropModel::insertRows(int row, int count, const QModelIndex &parent){
    if(row == 0)
    {
        while(count--){
            m_table.push_front(QList<QString>());
        }
        return true;
    }
    else if(row == rowCount())
    {
        while(count--){
            m_table.push_back(QList<QString>());
        }
        return true;
    }
    else
    {
        while (count--) {
            m_table.insert(row, QList<QString>());
        }
    }
    return false;
}

QList<QList<QString> > ShapePropModel::table()
{
    return m_table;
}
