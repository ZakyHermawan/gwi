#include <QDebug>
#include <fstream>

#include "DataManager.hpp"

DataManager::DataManager(fkyaml::node& root)
    : m_root{root}, m_currentIndex(0)
{
    for (const auto& dataNode : root["light_sensor_data"]) {
        int intensityValue = dataNode.as_int();

        if(intensityValue < 0) {
            throw std::runtime_error("Light intensity can't be negative");
        }

        m_intensityValues.append(intensityValue);
    }
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
        auto intensity = getIntensityByIndex(index);

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

float DataManager::getIntensityByIndex(int index)
{
    assert((index >= 0 && index <= 30) && "Index must be within 0 <= index <= 30");
    return m_intensityValues.at(index);
}

void DataManager::updateSensorReading(float lux)
{
    if (m_currentIndex < 0 || m_currentIndex >= 31) {
        throw std::runtime_error("Current index" + std::to_string(m_currentIndex) + "out of range");
    }
    
    // Update the value at the specified m_currentIndex
    m_intensityValues[m_currentIndex] = lux;
    
    try {
        if (m_root.contains("light_sensor_data") && m_root["light_sensor_data"].is_sequence()) {
            auto& sequence = m_root["light_sensor_data"].get_value_ref<fkyaml::node::sequence_type&>();
            if (m_currentIndex < static_cast<int>(sequence.size())) {
                sequence[m_currentIndex] = lux;
            }
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to update YAML node - " + std::string(e.what()));
    }
    
    /* Reached if absolutely no exception is thrown */
    qDebug() << "DataManager: Updated m_currentIndex" << m_currentIndex << "with value" << lux << "lx";
    emit dataUpdated(m_currentIndex, lux);
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
        m_currentIndex = (m_currentIndex + 1) % m_intensityValues.size();
        emit indexChanged(m_currentIndex);

    } catch (const std::runtime_error& e) {
        qFatal("DataManager: Fatal error - %s", e.what());
 
    }
    
}

void DataManager::resetIndex()
{
    m_currentIndex = 0;
    emit indexChanged(m_currentIndex);
    qDebug() << "DataManager: Reset index to 0";
}

int DataManager::getCurrentIndex() const
{
    return m_currentIndex;
}
