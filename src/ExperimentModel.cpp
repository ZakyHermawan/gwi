#include "ExperimentModel.hpp"

ExperimentModel::ExperimentModel(QList<QString> &sourceList, QSharedPointer<DataManager> dataManager, QObject *parent)
    : QAbstractListModel(parent), m_dataManager(dataManager), m_dataSource(&sourceList)
{
}

int ExperimentModel::rowCount(const QModelIndex &parent) const
{
    // Return the size of your actual list
    if (parent.isValid() || !m_dataSource)
        return 0;

    return m_dataSource->size();
}

QVariant ExperimentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_dataSource)
        return QVariant();

    // Ensure index is within bounds
    if (index.row() < 0 || index.row() >= m_dataSource->size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        QString original = m_dataSource->at(index.row());
        if (original.length() > 4) {
            return original.chopped(4);
        }
        return original;
    }

    if (role == RealFileNameRole) {
        return m_dataSource->at(index.row()); // Return full name
    }

    return QVariant();
}

// Optional: Only needed if you are using QML
QHash<int, QByteArray> ExperimentModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "experimentName";
    roles[RealFileNameRole] = "realFileName";
    return roles;
}

void ExperimentModel::refresh()
{
    // This tells the view that the entire list structure has changed
    beginResetModel();
    endResetModel();
}

void ExperimentModel::addEntry(const QString &name)
{
    if (name.isEmpty() || !m_dataSource) return;
    if (m_dataSource->contains(name)) return;

    int newRowIndex = m_dataSource->size();
    beginInsertRows(QModelIndex(), newRowIndex, newRowIndex);
    m_dataManager->addExperiment(name);
    m_dataManager->updateCurrentExperiment();
    m_dataSource = &m_dataManager->getExperimentNames();
    endInsertRows();
}

void ExperimentModel::removeEntry(int index) {
    QString nameToRemove = m_dataSource->at(index);
    int oldSize = m_dataSource->size();

    // Notify View of Removal
    beginRemoveRows(QModelIndex(), index, index);

    // Remove, and potentially Auto-Add if empty
    // This will modify m_dataSource (which is m_experimentNames in DataManager
    m_dataManager->removeExperiment(nameToRemove);

    // Finish Removal notification
    endRemoveRows();

    // Check if auto-add happened
    // If the size was 1, we removed it (Size 0), but DataManager auto-created one (Size 1).
    // The View thinks Size is 0, but DataManager has 1. We must notify insertion.
    if (oldSize == 1 && m_dataSource->size() == 1) {
        // We act as if a new row was inserted at index 0
        beginInsertRows(QModelIndex(), 0, 0);
        // No data change needed, DataManager already has it
        endInsertRows();

        // Force load the new auto-created experiment
        loadExperiment(0);
    }
    else if (m_dataSource->size() > 0) {
        // Normal delete case, just ensure data is synced
        m_dataManager->updateCurrentExperiment();
    }
}

void ExperimentModel::loadExperiment(int index)
{
    if (index < 0 || index >= m_dataSource->size()) return;

    QString experimentName = m_dataSource->at(index);

    m_dataManager->updateCurrentExperimentName(experimentName);
    m_dataManager->loadCurrentExperiment();
}
