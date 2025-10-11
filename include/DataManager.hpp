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

public slots:
    Q_INVOKABLE float getIntensityByIndex(int index);

    void updateSensorReading(int index, float lux);
    void addSensorReading(float lux);
    void resetIndex();
    
    // -- Get current index being updated
    Q_INVOKABLE int getCurrentIndex() const;

signals:
    // -- Signal when data at specific index is updated
    void dataUpdated(int index, float value);
    void indexChanged(int newIndex);
};
