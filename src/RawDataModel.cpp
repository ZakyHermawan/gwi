#include "RawDataModel.hpp"

RawDataModel::RawDataModel(QSharedPointer<DataManager> dataManager)
    : m_dataManager{dataManager}
{
    connect(m_dataManager.data(), &DataManager::dataUpdated,
            this, &RawDataModel::onDataUpdated);
}

int RawDataModel::rowCount(const QModelIndex &) const
{
    return 32;
}

int RawDataModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant RawDataModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        if(index.row() == 0)
        {
            // column name
            if(index.column() == 0)
            {
                return QString("Cycle");
            }
            else
            {
                return QString("Intensity Value (lx)");
            }
        }
        else
        {
            // the list of intensity values only contains 31 elements
            if(index.row() > 31)
            {
                return QString("");
            }

            // cycle number
            if(index.column() == 0)
            {
                return QString("%1").arg(index.row() - 1);
            }

            // intensity value
            auto value = m_dataManager->getIntensityByIndex(index.row() - 1);
            return QString("%1").arg(value);
        }

    case XValueRole:
        return index.row() - 1;

    case YValueRole:
        return m_dataManager->getIntensityByIndex(index.row() - 1);

    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> RawDataModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "displays"; // For raw data
    roles[XValueRole] = "xValue"; // For amplification plot
    roles[YValueRole] = "yValue"; // For amplification plot
    return roles;
}

void RawDataModel::onDataUpdated(int index, float value)
{
    int tableRow = index + 1; // +1 because row 0 is the header
    
    QModelIndex startIndex = createIndex(tableRow, 1);
    QModelIndex endIndex = startIndex;
    
    emit dataChanged(startIndex, endIndex, {Qt::DisplayRole});
}
