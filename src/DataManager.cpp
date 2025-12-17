#include <QDebug>
#include <fstream>

#include "DataManager.hpp"

DataManager::DataManager(fkyaml::node& root)
    : m_root{root},
    m_currentIntensityValuesIndex{0},
    m_concentrationCoefficient{"1"},
    m_concentrationMultiplier{1.f},
    m_rSquared{0.999},
    m_yIntercept{20.318},
    m_slope{-3.258},
    m_percentEfficiency{102.8},
    m_summary{"The resulting data is reliable for quantification."}
{
    for (const auto& dataNode : root["light_sensor_data"]) {
        int intensityValue = dataNode.as_int();

        if(intensityValue < 0) {
            throw std::runtime_error("Light intensity can't be negative");
        }

        m_intensityValues.append(intensityValue);
    }

    // Dummy standard curve data
    m_xyLogStandardCurve.append(qMakePair(0.7, 18));
    m_xyLogStandardCurve.append(qMakePair(-0.3, 22));
    m_xyLogStandardCurve.append(qMakePair(-1.3, 25));
    m_xyLogStandardCurve.append(qMakePair(-2.3, 28));
    m_xyLogStandardCurve.append(qMakePair(-3.3, 31));
    m_xyLogStandardCurve.append(qMakePair(-4.3, 34));

    calculateStandardCurve();
}

QList<float>& DataManager::getIntensityValuesList()
{
    return m_intensityValues;
}

void DataManager::save_data(std::string& fileName)
{
    std::ofstream ofs(fileName);

    // the element of root is interpreted as first sequence
    auto root = m_root[0];
    const float THRESHOLD = 0.1;

    uint8_t index = 0;
    for(auto& light_sensor_data: root["light_sensor_data"])
    {
        auto intensity = getIntensityValueByIndex(index);

        // handle case when number is too small (e.g. 2e-05)
        if(std::abs(intensity) < THRESHOLD)
        {
            intensity = 0.0f;
        }

        light_sensor_data = intensity;
        ++index;
    }
    ofs << root;
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
    
    try {
        if (m_root.contains("light_sensor_data") && m_root["light_sensor_data"].is_sequence()) {
            auto& sequence = m_root["light_sensor_data"].as_seq();
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
    /************************************************/
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
