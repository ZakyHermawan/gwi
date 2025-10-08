#include "ButtonHandler.hpp"
#include "StateManager.hpp"
#include "DataManager.hpp"
#include "SliderHandler.hpp"

#include "fkYAML.hpp"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QDir>

#include <iostream>
#include <cstdio>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    auto resourceFolderName = getenv("RESOURCE_FOLDER_PATH");
    auto mainQmlPath = QDir(resourceFolderName).filePath("../Main.qml");
    auto examplePath = QDir(resourceFolderName).filePath("example.yml");

    engine.load(mainQmlPath);
    auto fptr = fopen(examplePath.toStdString().c_str(), "r+");

    QTextStream in(fptr);
    std::string content = in.readAll().toStdString();

    fkyaml::node node = fkyaml::node::deserialize(content);

    ButtonHandler buttonHandler;
    SliderHandler sliderHandler;
    StateManager stateManager;
    DataManager dataManager(node);

    engine.rootContext()->setContextProperty("buttonHandler", &buttonHandler);
    engine.rootContext()->setContextProperty("sliderHandler", &sliderHandler);
    engine.rootContext()->setContextProperty("stateManager", &stateManager);
    engine.rootContext()->setContextProperty("dataManager", &dataManager);

    auto retval = app.exec();
    if(retval != 0)
    {
        std::cerr << "ERROR: Qt application exited with status code: " << retval << std::endl;
        exit(retval);
    }
    fclose(fptr);

    return 0;
}
