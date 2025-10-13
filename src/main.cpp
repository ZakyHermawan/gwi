#include "ButtonHandler.hpp"
#include "StateManager.hpp"
#include "DataManager.hpp"
#include "SliderHandler.hpp"
#include "HardwareController.hpp"

#include "RawDataModel.hpp"

#include "fkYAML.hpp"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QDir>
#include <QSharedPointer>

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
    SliderHandler sliderHandler(&app);
    StateManager stateManager;
    QSharedPointer<DataManager> dataManager{new DataManager(node)};
    RawDataModel rawDataModel(dataManager);
    HardwareController hardwareController(0x23, 1, 18, &app);

    // -- SliderHandler and HardwareController connections
    QObject::connect(&sliderHandler, &SliderHandler::ledIntensityRequested,
                     &hardwareController, &HardwareController::setLEDIntensity);
    
    // -- HardwareController and DataManager connections
    QObject::connect(&hardwareController, &HardwareController::sensorDataReady,
                 dataManager.data(), &DataManager::addSensorReading);

    if (!hardwareController.begin()) {
        std::cerr << "Main: Failed to initialize hardware!" << std::endl;
        return 1;
    }
    
    hardwareController.startSensorReading();

    engine.rootContext()->setContextProperty("buttonHandler", &buttonHandler);
    engine.rootContext()->setContextProperty("sliderHandler", &sliderHandler);
    engine.rootContext()->setContextProperty("stateManager", &stateManager);
    engine.rootContext()->setContextProperty("dataManager", dataManager.data());
    engine.rootContext()->setContextProperty("rawDataModel", &rawDataModel);

    auto retval = app.exec();
    if(retval != 0)
    {
        std::cerr << "ERROR: Qt application exited with status code: " << retval << std::endl;
        return retval;
    }
    fclose(fptr);

    return 0;
}
