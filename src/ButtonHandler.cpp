#include "ButtonHandler.hpp"

#include <QString>
#include <QDebug>
#include <QSharedPointer>
#include <QDir>

ButtonHandler::ButtonHandler(QSharedPointer<DataManager> dm, QObject* parent)
    : QObject(parent), m_dataManager{dm}
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
}

void ButtonHandler::saveDataClick()
{
    auto resourceFolderName = getenv("RESOURCE_FOLDER_PATH");
    auto examplePath = QDir(resourceFolderName).filePath("latest_data.yml");

    std::string savedFileName(examplePath.toStdString());
    m_dataManager->save_data(savedFileName);
}
