#include "StandardCurveModel.hpp"

StandardCurveModel::StandardCurveModel(QSharedPointer<DataManager> dataManager)
    : m_dataManager{dataManager}
{
    connect(m_dataManager.data(), &DataManager::xyLogStandardCurveUpdated,
            this, &StandardCurveModel::refreshModel);
}

int StandardCurveModel::rowCount(const QModelIndex &) const
{
    return m_dataManager->getXyLogStandardCurve().size();
}

int StandardCurveModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant StandardCurveModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    // Bounds check to prevent crashes
    auto standardCurvePoints = m_dataManager->getXyLogStandardCurve();
    if (index.row() < 0 || index.row() >= standardCurvePoints.size())
    {
        return QVariant();
    }

    auto point = standardCurvePoints[index.row()];

    // VXYModelMapper requests Qt::DisplayRole.
    // It distinguishes X and Y based on the column index you set in QML (xColumn: 0, yColumn: 1).
    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            return point.first;  // Return X for column 0
        }
        else if (index.column() == 1)
        {
            return point.second; // Return Y for column 1
        }
    }

    return QVariant();
}

QHash<int, QByteArray> StandardCurveModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[XValueRole] = "xValue"; // For standard curve plot
    roles[YValueRole] = "yValue"; // For standard curve plot
    return roles;
}

void StandardCurveModel::onStandardCurveUpdated(int index, float value)
{
    // Check if this index is a new row or an update to an existing row
    // Note: rowCount() calls m_dataManager->size().
    // If DataManager is already updated, rowCount() returns the NEW size.
    int currentSize = m_dataManager->getXyLogStandardCurve().size();

    // If the index matches the last element, we assume it's an append
    if (index == currentSize - 1)
    {
        // We must tell the view we are adding a row.
        // Even if DataManager already has the data, we strictly wrap the notification
        // so the View updates its internal mapping.

        // "index" is the position of the new row.
        beginInsertRows(QModelIndex(), index, index);
        endInsertRows();
    }
    else
    {
        // It is an update to an existing point
        QModelIndex startIndex = createIndex(index, 0); // Column 0 (X)
        QModelIndex endIndex = createIndex(index, 1);   // Column 1 (Y)

        // Notify that both columns changed
        emit dataChanged(startIndex, endIndex, {Qt::DisplayRole});
    }
}

void StandardCurveModel::refreshModel()
{
    beginResetModel();
    endResetModel();
}
