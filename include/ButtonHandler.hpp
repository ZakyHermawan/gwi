#pragma once

#include <QObject>
#include <QString>
#include <QSharedPointer>

#include "DataManager.hpp"
#include "HardwareController.hpp"

class ButtonHandler : public QObject
{
    Q_OBJECT
    QSharedPointer<DataManager> m_dataManager;
    QSharedPointer<HardwareController> m_hardwareController;

public:
    ButtonHandler(QSharedPointer<DataManager> dm, QSharedPointer<HardwareController> hwc, QObject* parent = nullptr);
    void handleRunStart();
    void handleRunStop();

signals:
    void exitApp();

public slots:
    void handleButtonClick(const QString &buttonName);
    void saveDataClick();
};
