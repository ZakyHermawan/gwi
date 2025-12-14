#pragma once

#include "DataManager.hpp"

#include <qqml.h>
#include <QAbstractTableModel>
#include <QList>

#include <QSharedPointer>
#include <QDebug>

class RawDataModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT

    // hold pointer to the data manager
    // to interact with light intensity data
    QSharedPointer<DataManager> m_dataManager;

public:
    enum AmplificationPlotRoles {
        XValueRole = Qt::UserRole + 1,
        YValueRole
    };

    RawDataModel(QSharedPointer<DataManager> dataManager);

    // 32 rows, 1 for the column name, 31 for cycle 0 until 31
    int rowCount(const QModelIndex & = QModelIndex()) const override;

    // 2 columns, for cycle number and intensity vaues
    int columnCount(const QModelIndex & = QModelIndex()) const override;

    // Returns the data stored under the given role for the item referred to by the index.
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

private slots:
    void onintensityValuesUpdated(int index, float value);
};
