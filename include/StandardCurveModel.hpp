#pragma once

#include "DataManager.hpp"

#include <qqml.h>
#include <QAbstractTableModel>
#include <QList>

#include <QSharedPointer>
#include <QDebug>

class StandardCurveModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT

    // Hold pointer to the data manager
    // to interact with light intensity data
    QSharedPointer<DataManager> m_dataManager;

public:
    enum StandardCurvesRoles {
        XValueRole = Qt::UserRole + 1,
        YValueRole
    };

    StandardCurveModel(QSharedPointer<DataManager> dataManager);

    int rowCount(const QModelIndex & = QModelIndex()) const override;

    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

public slots:
    void refreshModel();

private slots:
    void onStandardCurveUpdated(int index, float value);
};
