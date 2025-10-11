#include <QDebug>
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

float DataManager::getIntensityByIndex(int index)
{
    assert((index >= 0 && index <= 30) && "Index must be within 0 <= index <= 30");
    return m_intensityValues.at(index);
}

void DataManager::updateSensorReading(int index, float lux)
{
    if (index < 0 || index >= 31) {
        qDebug() << "DataManager: Cannot update index" << index << "- out of range";
        return;
    }
    
    // Update the value at the specified index
    m_intensityValues[index] = lux;
    
    try {
        if (m_root.contains("light_sensor_data") && m_root["light_sensor_data"].is_sequence()) {
            auto& sequence = m_root["light_sensor_data"].get_value_ref<fkyaml::node::sequence_type&>();
            if (index < static_cast<int>(sequence.size())) {
                sequence[index] = lux;
            }
        }
    } catch (const std::exception& e) {
        qDebug() << "DataManager: Failed to update YAML node:" << e.what();
    }
    
    qDebug() << "DataManager: Updated index" << index << "with value" << lux << "lx";
    
    // Emit signal to notify UI
    emit dataUpdated(index, lux);
}

void DataManager::addSensorReading(float lux)
{
    // -- Update the current index and cycle through 0-30
    updateSensorReading(m_currentIndex, lux);
    
		/**
     * Move to next index, cycle back to 0 after 30
		 * This acts as a foolproof so long as list is capped,
		 * though sensor reads only until 30 (HardwareController)
		 */
    m_currentIndex = (m_currentIndex + 1) % m_intensityValues.size();
    
    emit indexChanged(m_currentIndex);
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
