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

#include <type_traits>

// Take variadic number of FILE* as argument, then close each of them.
template<typename... Args>
static void close_files(Args... files) {
    // Make sure all argument is FILE*.
    static_assert((std::is_same_v<Args, FILE*> && ...),
                  "All arguments must be of type FILE*");

    // This lambda will be called for each file pointer.
    auto close_if_valid = [](FILE* f) {
        if (f != nullptr) {
            std::cout << "Closing file..." << std::endl;
            fclose(f);
        }
    };

    // The fold expression applies the lambda to each argument.
    (close_if_valid(files), ...);
}

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
    auto lastLedIntensity = QDir(resourceFolderName).filePath("last_led_intensity.txt");

    engine.load(mainQmlPath);
    auto exampleFptr = fopen(examplePath.toStdString().c_str(), "r+");
    auto ledFptr = fopen(lastLedIntensity.toStdString().c_str(), "r+");

    QTextStream exampleIn(exampleFptr);
    QTextStream ledIn(ledFptr);

    auto exampleContent = exampleIn.readAll().toStdString();
    bool ok;
    auto ledContent = ledIn.readLine().trimmed().toUInt(&ok);

    if (!ok) {
        qFatal() << "Conversion failed, but the original line was suspicious.";
        close_files(exampleFptr, ledFptr);
        return 1;
    }

    fkyaml::node node = fkyaml::node::deserialize(exampleContent);

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
        close_files(exampleFptr, ledFptr);
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
        close_files(exampleFptr, ledFptr);
        return retval;
    }

    close_files(exampleFptr, ledFptr);
    return 0;
}
