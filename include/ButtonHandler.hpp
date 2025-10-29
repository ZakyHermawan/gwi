#pragma once

#include <QObject>
#include <QString>
#include <QSharedPointer>

#include "DataManager.hpp"

class ButtonHandler : public QObject
{
    Q_OBJECT
    QSharedPointer<DataManager> m_dataManager;

public:
    ButtonHandler(QSharedPointer<DataManager> dm, QObject* parent = nullptr);
    void handleRun();

public slots:
    void handleButtonClick(const QString &buttonName);
    void saveDataClick();
};
