#pragma once

#include "fkYAML.hpp"

#include <QObject>
#include <QList>

class DataManager : public QObject
{
    Q_OBJECT
    fkyaml::node m_root;
    QList<float> m_intensityValues;

public:
    DataManager() = default;
    DataManager(fkyaml::node& root);
    QList<float>& getIntensityValuesList();

public slots:
    Q_INVOKABLE float getIntensityByIndex(int index);
};
