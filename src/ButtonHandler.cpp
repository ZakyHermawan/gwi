#include "ButtonHandler.hpp"

#include <QString>
#include <QDebug>
#include <QSharedPointer>
#include <QDir>
#include <QApplication>

#include <cstdlib>

ButtonHandler::ButtonHandler(QSharedPointer<DataManager> dm, QSharedPointer<HardwareController> hwc, QObject* parent)
    : QObject(parent), m_dataManager{dm}, m_hardwareController{hwc}
{
}

void ButtonHandler::handleButtonClick(const QString &buttonName)
{
    // The C++ event filter has already updated the button's text and the global state.
    // We dispatch based on the *new* text value:

    if (buttonName == "Stop")
    {
        // User clicked "Run" -> Text changed to "Stop" -> START the hardware.
        handleRunStart();
    }
    else if (buttonName == "Run")
    {
        // User clicked "Stop" -> Text changed back to "Run" -> STOP the hardware.
        handleRunStop();
    }
    else if(buttonName == "End")
    {
#ifdef __linux__
        // just shutdown the system if this is linux (embedded)
        system("shutdown -P now");
#else
        QApplication::quit();
#endif
    }
    else {
        // Handle other menu buttons (Setup, Analysis, etc.)
        qDebug() << "Button clicked:" << buttonName;
    }
}

void ButtonHandler::handleRunStart()
{
    m_dataManager->resetIntensityValues();
    if (!m_hardwareController->begin()) {
        throw std::runtime_error("Main: Failed to initialize hardware!");
    }

    m_hardwareController->startSensorReading();
}

void ButtonHandler::handleRunStop()
{
    m_hardwareController->stopSensorReading();
}

void ButtonHandler::saveDataClick()
{
    auto resourceFolderName = getenv("RESOURCE_FOLDER_PATH");
    auto examplePath = QDir(resourceFolderName).filePath("latest_data.yml");

    std::string savedFileName(examplePath.toStdString());
    m_dataManager->save_data(savedFileName);
}
