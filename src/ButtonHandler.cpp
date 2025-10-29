#include "ButtonHandler.hpp"

#include <QString>
#include <QDebug>
#include <QSharedPointer>
#include <QDir>

ButtonHandler::ButtonHandler(QSharedPointer<DataManager> dm, QSharedPointer<HardwareController> hwc, QObject* parent)
    : QObject(parent), m_dataManager{dm}, m_hardwareController{hwc}
{
}

void ButtonHandler::handleButtonClick(const QString &buttonName)
{
    if(buttonName == "Run")
    {
        handleRun();
    }
}

void ButtonHandler::handleRun()
{
    // start experiment here
    if (!m_hardwareController->begin()) {
        throw std::runtime_error("Main: Failed to initialize hardware!");
    }

    m_hardwareController->startSensorReading();
}

void ButtonHandler::saveDataClick()
{
    auto resourceFolderName = getenv("RESOURCE_FOLDER_PATH");
    auto examplePath = QDir(resourceFolderName).filePath("latest_data.yml");

    std::string savedFileName(examplePath.toStdString());
    m_dataManager->save_data(savedFileName);
}
