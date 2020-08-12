#ifndef SHAPEPROPMODEL_H
#define SHAPEPROPMODEL_H

#include <QAbstractTableModel>

class ShapePropModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ShapePropModel(QList<QList<QString>> table, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    QList<QList<QString>> table();

private:
    QList<QList<QString>> m_table;
};

#endif // SHAPEPROPMODEL_H
