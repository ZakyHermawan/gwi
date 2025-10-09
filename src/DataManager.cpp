#include "DataManager.hpp"

DataManager::DataManager(fkyaml::node& root)
    : m_root{root}
{
    for (const auto& dataNode : root["light_sensor_data"]) {
        int intensityValue = dataNode.as_int();

        if(intensityValue < 0) {
            throw std::runtime_error("Light intensity can't be negative");
        }

        m_intensityValues.append(intensityValue);
    }
}

float DataManager::getIntensityByIndex(int index)
{
    assert((index >= 0 && index <= 30) && "Index must be within 0 <= index <= 30");
    return m_intensityValues.at(index);
}
