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

    // Hold pointer to the data manager
    // to interact with light intensity data
    QSharedPointer<DataManager> m_dataManager;

public:
    enum AmplificationPlotRoles {
        XValueRole = Qt::UserRole + 1,
        YValueRole
    };

    RawDataModel(QSharedPointer<DataManager> dataManager);

    // first row is for the labels
    int rowCount(const QModelIndex & = QModelIndex()) const override;

    // 2 columns, one for cycle number and one for intensity vaues
    int columnCount(const QModelIndex & = QModelIndex()) const override;

    // Returns the data stored under the given role for the item referred to by the index.
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

public slots:
    void onIntensityValuesUpdated(int index, float value);
    void refresh();
};
