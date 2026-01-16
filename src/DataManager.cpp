#include <QDebug>

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <QDir>

#include "DataManager.hpp"

DataManager::DataManager(QMap<QString, fkyaml::node>& experiments)
    : m_experiments{experiments},
    m_currentIntensityValuesIndex{0},
    m_cycleThreshold{0},
    m_ledIntensity{0},
    m_maxCycle{0},
    m_currentExperimentName{""},
    m_concentrationCoefficient{"1"},
    m_concentrationMultiplier{1.f},
    m_rSquared{0.999},
    m_yIntercept{20.318},
    m_slope{-3.258},
    m_percentEfficiency{102.8},
    m_lastSaved{""},
    m_summary{"The resulting data are not reliable for quantification."}
{
    for(const auto& [experimentName, value] : m_experiments.asKeyValueRange())
    {
        m_experimentNames.push_back(experimentName);
    }

    if (m_experimentNames.isEmpty()) {
        qDebug() << "No experiments found on startup. Creating 'new_experiment.yml'...";
        createExperimentFromTemplate("new_experiment.yml");
    }

    // Safety: Ensure we have a valid current name
    if (m_currentExperimentName.isEmpty() && !m_experimentNames.isEmpty()) {
        m_currentExperimentName = m_experimentNames.first();
    }

    // Only load if we actually have data (prevents crash if template creation failed)
    if (!m_experimentNames.isEmpty()) {
        loadCurrentExperiment();
    }
}

QList<QString>& DataManager::getExperimentNames()
{
    return m_experimentNames;
}

QList<float>& DataManager::getIntensityValuesList()
{
    return m_intensityValues;
}

void DataManager::save_data()
{
    auto experimentName = m_currentExperimentName.toStdString();

    // add new file or rewrite file with name m_currentExperimentName
    auto resourceFolderName = getenv("RESOURCE_FOLDER_PATH");
    QDir dir = QDir(resourceFolderName).filePath("experiments");
    QString absolutePath = dir.absoluteFilePath(m_currentExperimentName);
    std::ofstream ofs(absolutePath.toStdString());

    m_currentExperimentName = QString::fromStdString(experimentName);
    updateCurrentExperiment(); // assign all private members to fkyaml node of current experiment
    m_currentExperiment = m_experiments[m_currentExperimentName];

    // Manually assign to fkyaml node for every data that is not assigned by updateCurrentExperiment
    m_currentExperiment["last_saved"] = getCurrTimeStampStr().toStdString();
    m_currentExperiment["r_squared"] = m_rSquared;
    m_currentExperiment["slope"] = m_slope;
    m_currentExperiment["cycle_threshold"] = m_cycleThreshold;
    m_currentExperiment["y_intercept"] = m_yIntercept;
    m_currentExperiment["efficiency"] = m_percentEfficiency;
    m_currentExperiment["experiment_name"] = m_currentExperimentName.chopped(4).toStdString();
    m_currentExperiment["summary"] = m_summary.toStdString();

    // store sensor data
    auto& sequence = m_currentExperiment["light_sensor_data"].as_seq();
    sequence.clear();
    for(int i = 0; i<m_maxCycle; ++i)
    {
        sequence.push_back(m_intensityValues[i]);
    }

    auto& curve_points = m_currentExperiment["standard_curve_points"].as_seq();
    curve_points.clear();
    for(int i = 0; i<m_xyLogStandardCurve.size(); ++i)
    {
        curve_points.push_back(m_xyLogStandardCurve[i]);
    }

    // Write to the file
    ofs << m_currentExperiment;
}

std::tuple<double, double, double>
DataManager::simpleLinearRegression(const std::vector<double>& x, const std::vector<double>& y) {
    if (x.size() != y.size() || x.empty()) return {0.0, 0.0, 0.0};

    size_t n = x.size();
    if (n < 2) return {0.0, 0.0, 0.0}; // Need at least 2 points

    // Compute Means (Averages)
    // This centers the data, keeping numbers smaller to avoid overflow.
    double sum_x = 0.0;
    double sum_y = 0.0;
    for (size_t i = 0; i < n; ++i) {
        sum_x += x[i];
        sum_y += y[i];
    }
    double mean_x = sum_x / n;
    double mean_y = sum_y / n;

    // Compute Variance & Covariance
    // We sum the differences from the mean, rather than raw squares.
    // This prevents Catastrophic Cancellation.
    double ss_xx = 0.0; // Sum of squares (x - mean_x)
    double ss_yy = 0.0; // Sum of squares (y - mean_y)
    double ss_xy = 0.0; // Sum of products (x - mean_x)*(y - mean_y)

    for (size_t i = 0; i < n; ++i) {
        double dx = x[i] - mean_x;
        double dy = y[i] - mean_y;

        ss_xx += dx * dx;
        ss_yy += dy * dy;
        ss_xy += dx * dy;
    }

    // If ss_xx is 0, all X values are identical (vertical line).
    if (std::abs(ss_xx) < 1e-9) return {0.0, 0.0, 0.0};

    double slope = ss_xy / ss_xx;
    double intercept = mean_y - slope * mean_x;

    // R^2 = (Covariance / (StdDev_X * StdDev_Y))^2
    double r_squared = 0.0;
    if (ss_yy > 1e-9) {
        // Mathematically equivalent to: (ss_xy * ss_xy) / (ss_xx * ss_yy)
        // Calculated this way to preserve sign/precision logic
        double r = ss_xy / std::sqrt(ss_xx * ss_yy);
        r_squared = r * r;
    }

    return {slope, intercept, r_squared};
}

double DataManager::calculatePCREfficiency(double slope) {
    // Prevent division by zero
    // A slope of 0 means Ct never changes regardless of dilution (impossible/bad data).
    if (std::abs(slope) < 1e-9) {
        return std::numeric_limits<double>::infinity();
    }

    double exponent = -1.0 / slope;

    // std::pow(10, x) will overflow 'double' if x > ~308.
    // This happens if the slope is extremely small (e.g., -0.000001).
    if (exponent > 308.0) {
        return std::numeric_limits<double>::infinity();
    }

    // Calculate Efficiency
    // Subtract 1 to get the efficiency fraction, multiply by 100 for percentage.
    return (std::pow(10.0, exponent) - 1.0) * 100.0;
}

void DataManager::calculateStandardCurve()
{
    if(m_xyLogStandardCurve.size() < 5) {
        m_rSquared = 0.0f;
        m_yIntercept = 0.0f;
        m_slope = 0.0f;
        m_percentEfficiency = 0.0f;
        m_cycleThreshold = 0;
        m_summary = "You need at least 5 dilution points.";
        return;
    }

    std::vector<double> x, y;
    for(int i=0; i<m_xyLogStandardCurve.size(); ++i)
    {
        x.push_back(m_xyLogStandardCurve[i].first);
        y.push_back(m_xyLogStandardCurve[i].second);
    }

    auto [slope, intercept, r_squared] = simpleLinearRegression(x, y);
    double efficiency = calculatePCREfficiency(slope);

    m_slope = slope;
    m_yIntercept = intercept;
    m_rSquared = r_squared;
    m_percentEfficiency = efficiency;

    if(m_rSquared >= 0.98)
    {
        m_summary = "The resulting data are reliable for quantification.";
    }
    else
    {
        m_summary = "The resulting data are not reliable for quantification.";
    }

}

void DataManager::resetIntensityValues()
{
    m_currentIntensityValuesIndex = 0;
    for(uint8_t i = 0; i < m_intensityValues.size(); ++i)
    {
        m_intensityValues[i] = 0.0f;
    }
}

void DataManager::resetStandardCurveData()
{
    m_rSquared = 0.0f;
    m_yIntercept = 0.0f;
    m_slope = 0.0f;
    m_percentEfficiency = 0.0f;
    m_summary = "";
    m_cycleThreshold = 0;
    m_xyLogStandardCurve.clear();
    calculateStandardCurve();
}

void DataManager::setIntensityValuesSize(int size)
{
    m_intensityValues.resize(size);
}

QList<QPair<double, int>>& DataManager::getXyLogStandardCurve()
{
    return m_xyLogStandardCurve;
}

int DataManager::getIntensityValuesSize() const
{
    return m_intensityValues.size();
}

int DataManager::getStandardCurveDataSize() const
{
    return m_xyLogStandardCurve.size();
}

float DataManager::getConcentrationMultiplier() const
{
    return m_concentrationMultiplier;
}

void DataManager::setConcentrationMultiplier(float multiplier)
{
    m_concentrationMultiplier = multiplier;
}

float DataManager::getIntensityValueByIndex(int index)
{
    if(index < 0 || index >= getIntensityValuesSize())
    {
        throw std::runtime_error("invalid intensityValues index access at " + std::to_string(index));
    }
    return m_intensityValues.at(index);
}

void DataManager::updateSensorReading(float lux)
{
    if (m_currentIntensityValuesIndex < 0 || m_currentIntensityValuesIndex >= 31) {
        throw std::runtime_error("Current index" + std::to_string(m_currentIntensityValuesIndex) + "out of range");
    }
    
    // Update the value at the specified m_currentIntensityValuesIndex
    m_intensityValues[m_currentIntensityValuesIndex] = lux;
    auto& currentExperiment = m_experiments[m_currentExperimentName];
    try {
        if (currentExperiment.contains("light_sensor_data") && currentExperiment["light_sensor_data"].is_sequence()) {
            auto& sequence = currentExperiment["light_sensor_data"].as_seq();
            if (m_currentIntensityValuesIndex < static_cast<int>(sequence.size())) {
                sequence[m_currentIntensityValuesIndex] = lux;
            }
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to update YAML node - " + std::string(e.what()));
    }
    
    /* Reached if absolutely no exception is thrown */
    qDebug() << "DataManager: Updated m_currentIntensityValuesIndex"
             << m_currentIntensityValuesIndex
             << "with value" << lux << "lx";

    emit intensityValuesUpdated(m_currentIntensityValuesIndex, lux);
}

void DataManager::addSensorReading(float lux)
{
    // -- Update the current index and cycle through 0-30
    try {
        updateSensorReading(lux);
        /**
        * Move to next index, cycle back to 0 after 30
        * This acts as a foolproof so long as list is capped,
        * though sensor reads only until 30 (HardwareController)
        */
        m_currentIntensityValuesIndex = (m_currentIntensityValuesIndex + 1) % m_intensityValues.size();
        emit currentIntensityValuesIndexChanged(m_currentIntensityValuesIndex);

    } catch (const std::runtime_error& e) {
        qFatal("DataManager: Fatal error - %s", e.what());
    }
}

int DataManager::getCurrentIntensityValuesIndex() const
{
    return m_currentIntensityValuesIndex;
}

QString DataManager::getCurrTimeStampStr()
{
    using namespace std::chrono;
    auto now = system_clock::now();
    // Add 7 hours for UTC+7
    auto now_plus_7 = now + hours(7);

    // Convert to time_t to get calendar components
    auto time_t_val = system_clock::to_time_t(now_plus_7);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // Format the string: "YYYY-MM-DD HH:MM:SS.ms +7"
    // Note: We use gmtime because we manually added the 7 hours offset above
    std::tm tm_val;

// Cross-platform gmtime (use gmtime_s on Windows, gmtime_r on Linux/macOS)
#if defined(_WIN32)
    gmtime_s(&tm_val, &time_t_val);
#else
    gmtime_r(&time_t_val, &tm_val);
#endif

    std::stringstream ss;
    ss << std::put_time(&tm_val, "%Y-%m-%d %H:%M:%S");

    // Add milliseconds (ensure 2 digits as requested in your format example ".78")
    // Usually 3 digits (.780) is standard, but here is standard streaming:
    ss << "." << std::setfill('0') << std::setw(2) << (ms.count() / 10);
    ss << " +7";

    return QString::fromStdString(ss.str());
}

void DataManager::setCycleThreshold()
{
    for(unsigned int i = 0; i < m_intensityValues.size(); ++i)
    {
        if (m_intensityValues[i] >= m_intensityThreshold)
        {
            m_cycleThreshold = i + 1;
            return;
        }
    }
}

int DataManager::getInitialLedIntensityValue()
{
    return m_ledIntensity;
}

void DataManager::setInitialLedIntensityValue(int ledIntensityValue)
{
    m_ledIntensity = ledIntensityValue;
}

void DataManager::setMaxCycle(int maxCycle)
{
    m_maxCycle = maxCycle;
}

void DataManager::updateCurrentExperiment()
{
    updateLedIntensity();
    updateMaxCycle();
    updateIntensityThreshold();
    updateCycleThreshold();
    updateConcentrationCoefficient();
    updateConcentrationMultiplier();
    updateXYStandardCurve();
}

void DataManager::updateLedIntensity()
{
    auto& currentExperiment = m_experiments[m_currentExperimentName];
    currentExperiment["led_intensity_level"] = m_ledIntensity;
}

void DataManager::updateMaxCycle()
{
    auto& currentExperiment = m_experiments[m_currentExperimentName];
    currentExperiment["max_cycle"] = m_maxCycle;
}

void DataManager::updateIntensityThreshold()
{
    auto& currentExperiment = m_experiments[m_currentExperimentName];
    currentExperiment["intensity_threshold"] = m_intensityThreshold;
}

void DataManager::updateCycleThreshold()
{
    auto& currentExperiment = m_experiments[m_currentExperimentName];
    currentExperiment["cycle_threshold"] = m_cycleThreshold;
}

void DataManager::updateConcentrationCoefficient()
{
    auto& currentExperiment = m_experiments[m_currentExperimentName];
    currentExperiment["concentration_coefficient"] = m_concentrationCoefficient.toFloat();
}

void DataManager::updateConcentrationMultiplier()
{
    auto& currentExperiment = m_experiments[m_currentExperimentName];
    currentExperiment["concentration_multiplier"] = m_concentrationMultiplier;
}

void DataManager::updateXYStandardCurve()
{
    auto& currentExperiment = m_experiments[m_currentExperimentName];
    bool initiallyEmpty = m_xyLogStandardCurve.size();
    for(size_t i = 0; i < m_xyLogStandardCurve.size(); ++i)
    {
        if(initiallyEmpty)
        {
            currentExperiment["standard_curve_points"].as_seq().push_back(m_xyLogStandardCurve[i]);
        }
        else
        {
            currentExperiment["standard_curve_points"].as_seq()[i] = m_xyLogStandardCurve[i];
        }
    }
}

void DataManager::updateCurrentExperimentName(QString& currentExperimentName)
{
    m_currentExperimentName = currentExperimentName;
}

void DataManager::loadCurrentExperiment()
{
    // Assign members with values from experiment node
    // Emit signal to update the value in the UI
    auto& root = m_experiments[m_currentExperimentName];

    m_lastSaved = root["last_saved"].as_str();
    m_ledIntensity = root["led_intensity_level"].as_int();
    emit ledIntensityChanged();

    m_maxCycle = root["max_cycle"].as_int();
    setIntensityValuesSize(m_maxCycle);
    emit maxCycleChanged();

    m_intensityThreshold = root["intensity_threshold"].as_float();
    emit intensityThresholdChanged();

    m_cycleThreshold = root["cycle_threshold"].as_int();
    emit cycleThresholdChanged();

    m_concentrationCoefficient = QString::number(root["concentration_coefficient"].as_float());
    emit concentrationCoefficientChanged();

    m_concentrationMultiplier = root["concentration_multiplier"].as_float();
    emit concentrationMultiplierChanged();

    m_summary = QString::fromStdString(root["summary"].as_str());
    emit summaryChanged();

    m_xyLogStandardCurve.clear();
    if (root["standard_curve_points"].as_seq().size() == 0)
    {
        calculateStandardCurve();
        emit xyLogStandardCurveUpdated();
    }
    for(auto& point: root["standard_curve_points"].as_seq())
    {
        double x = point[0].as_float();
        double y = point[1].as_int();
        m_xyLogStandardCurve.append(qMakePair(x, y));
    }
    std::sort(m_xyLogStandardCurve.begin(), m_xyLogStandardCurve.end(),
              [](const QPair<double, int>& a, const QPair<double, int>& b) {
                  return a.first < b.first;
              });
    calculateStandardCurve();
    emit xyLogStandardCurveUpdated();
}

void DataManager::removeExperiment(const QString experimentName)
{
    // Check for duplicate
    int idx = -1;
    for(int i=0; i<m_experimentNames.size(); ++i)
    {
        if(m_experimentNames[i] == experimentName)
        {
            idx = i;
            break;
        }
    }
    if(idx == -1) return;

    // Remove from Map and List
    m_experiments.remove(experimentName);
    m_experimentNames.removeAt(idx);

    // Remove File
    auto resourceFolderName = getenv("RESOURCE_FOLDER_PATH");
    QDir dir = QDir(resourceFolderName).filePath("experiments");
    QString absolutePath = dir.absoluteFilePath(experimentName);
    QFile::remove(absolutePath);

    // Check if we have deleted the last item
    if (m_experimentNames.isEmpty())
    {
        // Create "new_experiment.yml" from template
        // This adds it to the list and sets m_currentExperimentName
        createExperimentFromTemplate("new_experiment.yml");
    }
    else
    {
        // Adjust index if we deleted the last item
        if (idx >= m_experimentNames.size()) {
            idx = m_experimentNames.size() - 1;
        }
        m_currentExperimentName = m_experimentNames.at(idx);
        loadCurrentExperiment();
    }
}


void DataManager::addExperiment(const QString experimentName)
{
    // Note: ExperimentName already have .yml extension
    // Check duplicates
    for(const auto& name : qAsConst(m_experimentNames))
    {
        if(name == experimentName) return;
    }
    createExperimentFromTemplate(experimentName);
    save_data();
}

void DataManager::resetCurrentExperiment()
{
    // Reset member values
    m_currentIntensityValuesIndex = 0;
    m_cycleThreshold = 0;
    m_ledIntensity = 0;
    m_maxCycle = 0;
    m_concentrationCoefficient = "1";
    m_concentrationMultiplier = 1.f;
    m_rSquared =0.0f;
    m_yIntercept = 0.0f;
    m_slope = 0.0f;
    m_percentEfficiency = 0.0f;
    m_lastSaved = "";
    m_summary = "The resulting data are not reliable for quantification.";

    resetIntensityValues();
    resetStandardCurveData();

    // assign current experiment node with values from members
    updateCurrentExperiment();
}

void DataManager::createExperimentFromTemplate(const QString& experimentName)
{
    auto resourceFolderName = getenv("RESOURCE_FOLDER_PATH");
    if (!resourceFolderName) {
        qWarning() << "RESOURCE_FOLDER_PATH not set!";
        return;
    }

    QDir dir = QDir(resourceFolderName).filePath("experiments");
    QString absoluteEmptyPath = dir.absoluteFilePath("../empty_experiment.yml");
    QString absolutePath = dir.absoluteFilePath(experimentName);

    std::ifstream templateFile(absoluteEmptyPath.toStdString());
    if (!templateFile.is_open()) {
        qCritical() << "Could not find empty_experiment.yml at" << absoluteEmptyPath;
        return;
    }

    try {
        fkyaml::node root = fkyaml::node::deserialize(templateFile);
        root["experiment_name"] = experimentName.chopped(4).toStdString();
        root["last_saved"] = getCurrTimeStampStr().toStdString();

        m_experiments[experimentName] = root;
        m_experimentNames.push_back(experimentName);
        m_currentExperimentName = experimentName;

        // Update members to the values from current experiment map
        loadCurrentExperiment();

        // Write to Disk
        std::ofstream newFile(absolutePath.toStdString());
        if (newFile.is_open()) {
            newFile << root;
            newFile.close();
        } else {
            qCritical() << "Failed to create new experiment file";
        }

        // Load data into C++ members and notify UI
        // (This ensures sliders/text fields update to the new values)
        loadCurrentExperiment();
        updateCurrentExperiment();
    } catch (const fkyaml::exception& e) {
        qCritical() << "fkYAML Error:" << e.what();
    }
}
