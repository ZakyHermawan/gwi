#pragma once

#include <QAbstractListModel>
#include <QString>
#include <QList>
#include <QSharedPointer>

#include "DataManager.hpp"

class ExperimentModel : public QAbstractListModel
{
    Q_OBJECT

public:
    // Constructor takes a reference to your existing list
    explicit ExperimentModel(QList<QString> &sourceList, QSharedPointer<DataManager> dataManager, QObject *parent = nullptr);

    // Required overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Optional: If you want to use this in QML
    QHash<int, QByteArray> roleNames() const override;
    void refresh();
    Q_INVOKABLE void addEntry(const QString &name);
    Q_INVOKABLE void removeEntry(int index);
    Q_INVOKABLE void loadExperiment(int index);

private:
    // We store a pointer to the list inside DataManager
    QList<QString> *m_dataSource;
    QSharedPointer<DataManager> m_dataManager;

    enum ExperimentRoles {
        RealFileNameRole = Qt::UserRole + 1
    };
};
