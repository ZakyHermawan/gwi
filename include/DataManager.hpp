#pragma once

#include "fkYAML.hpp"

#include <QObject>
#include <QList>

class DataManager : public QObject
{
    Q_OBJECT

    fkyaml::node m_root;
    QList<float> m_intensityValues;
    int m_currentIndex;

public:
    DataManager() = default;
    DataManager(fkyaml::node& root);
    QList<float>& getIntensityValuesList();
    void save_data(std::string& fileName);
    void resetIntensityValues();

public slots:
    Q_INVOKABLE float getIntensityByIndex(int index);
    void addSensorReading(float lux);
    void resetIndex();
    void setIntensityValuesSize(int size);
    int getIntensityValuesSize();

    // -- Get current index being updated
    Q_INVOKABLE int getCurrentIndex() const;

private slots:
    void updateSensorReading(float lux);

signals:
    // -- Signal when data at specific index is updated
    void dataUpdated(int index, float value);
    void indexChanged(int newIndex);
};
