#include "RawDataModel.hpp"

RawDataModel::RawDataModel(QSharedPointer<DataManager> dataManager)
    : m_dataManager{dataManager}
{
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
                return QString("Intensity Value");
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

    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> RawDataModel::roleNames() const
{
    return { {Qt::DisplayRole, "displays"} };
}
