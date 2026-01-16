#include "ButtonHandler.hpp"
#include "StateManager.hpp"
#include "DataManager.hpp"
#include "SliderHandler.hpp"
#include "HardwareController.hpp"
#include "RawDataModel.hpp"
#include "ExperimentModel.hpp"
#include "StandardCurveModel.hpp"
#include "RunButtonlEventFilter.hpp"

#include "fkYAML.hpp"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QDir>
#include <QSharedPointer>
#include <QQuickWindow>
#include <QListWidgetItem>
#include <QMap>

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
            fclose(f);
        }
    };

    // The fold expression applies the lambda to each argument.
    (close_if_valid(files), ...);
}

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
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

    QDir dir = QDir(resourceFolderName).filePath("experiments");
    QStringList fileNames = dir.entryList(QStringList() << "*.yml", QDir::Files);

    QMap<QString, fkyaml::node> experiments;
    for(const auto& fileName: qAsConst(fileNames))
    {
        QFile file(dir.absoluteFilePath(fileName));
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            std::string content = file.readAll().toStdString();
            experiments[fileName] = fkyaml::node::deserialize(content);
        }
    }

    StateManager stateManager;
    QSharedPointer<DataManager> dataManager(new DataManager(experiments));

    SliderHandler sliderHandler(dataManager, &app);
    RawDataModel rawDataModel(dataManager);
    ExperimentModel experimentModel(dataManager->getExperimentNames(), dataManager);
    StandardCurveModel standardCurveModel(dataManager);

    // HardwareController hardwareController(0x23, 1, 18, &app);
    QSharedPointer<HardwareController> hardwareController(new HardwareController(0x23, 1, 18, &app));
    ButtonHandler buttonHandler(dataManager, hardwareController);

    // SliderHandler and HardwareController connections
    QObject::connect(&sliderHandler, &SliderHandler::ledIntensityRequested,
                     hardwareController.data(), &HardwareController::setLEDIntensity);
    
    // HardwareController and DataManager connections
    QObject::connect(hardwareController.data(), &HardwareController::sensorDataReady,
                 dataManager.data(), &DataManager::addSensorReading);

    // Shutdown OS (linux) or close app (non-linux) when end button is pressed
    QObject::connect(&buttonHandler, &ButtonHandler::exitApp, &app, QApplication::closeAllWindows, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("buttonHandler", &buttonHandler);
    engine.rootContext()->setContextProperty("sliderHandler", &sliderHandler);
    engine.rootContext()->setContextProperty("stateManager", &stateManager);
    engine.rootContext()->setContextProperty("dataManager", dataManager.data());

    engine.rootContext()->setContextProperty("rawDataModel", &rawDataModel);
    engine.rootContext()->setContextProperty("experimentModel", &experimentModel);
    engine.rootContext()->setContextProperty("standardCurveModel", &standardCurveModel);
    engine.load(mainQmlPath);

    // install run button event filter
    QObjectList rootObjects = engine.rootObjects();
    if (!rootObjects.isEmpty()) {
        QObject* rootItem = rootObjects.first();
        QObject* runButton = rootItem->findChild<QObject*>("runButton");

        if (runButton) {
            RunButtonEventFilter* RunButtonFilter = new RunButtonEventFilter(&app);
            runButton->installEventFilter(RunButtonFilter); // Install filter directly on the button
        } else {
            qWarning() << "Could not find 'runButton' object in the QML hierarchy to install event filter. Ensure it is defined and fully loaded.";
        }
    }

    auto retval = app.exec();
    if(retval != 0)
    {
        std::cerr << "ERROR: Qt application exited with status code: " << retval << std::endl << std::flush;
        return retval;
    }
    return 0;
}
