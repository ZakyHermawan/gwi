#pragma once

#include "fkYAML.hpp"

#include <QObject>
#include <QList>

class DataManager : public QObject
{
    Q_OBJECT

    fkyaml::node m_root;
    QList<float> m_intensityValues;
    int m_currentIntensityValuesIndex;

    // Use QString for display on setup,
    // convert to float later for processing
    QString m_concentrationCoefficient;
    float m_concentrationMultiplier;

    float m_rSquared;
    float m_yIntercept;
    float m_slope;
    float m_percentEfficiency;
    QString m_summary;

    Q_PROPERTY(QString concentrationCoefficient
        MEMBER m_concentrationCoefficient
        NOTIFY concentrationCoefficientChanged)

    Q_PROPERTY(float concentrationMultiplier
        MEMBER m_concentrationMultiplier
        NOTIFY concentrationMultiplierChanged)

    Q_PROPERTY(float rSquared
        MEMBER m_rSquared
        NOTIFY rSquaredChanged)

    Q_PROPERTY(float yIntercept
        MEMBER m_yIntercept
        NOTIFY yInterceptChanged)

    Q_PROPERTY(float slope
        MEMBER m_slope
        NOTIFY slopeChanged)

    Q_PROPERTY(float percentEfficiency
        MEMBER m_percentEfficiency
        NOTIFY percentEfficiencyChanged)

    Q_PROPERTY(QString summary
        MEMBER m_summary
        NOTIFY summaryChanged)

public:
    DataManager() = default;
    DataManager(fkyaml::node& root);
    QList<float>& getIntensityValuesList();
    void save_data(std::string& fileName);
    void resetIntensityValues();

public slots:
    Q_INVOKABLE float getIntensityValueByIndex(int index);
    void addSensorReading(float lux);
    void setIntensityValuesSize(int size);
    Q_INVOKABLE int getIntensityValuesSize() const;

    Q_INVOKABLE float getConcentrationMultiplier() const;
    Q_INVOKABLE void setConcentrationMultiplier(float multiplier);

    Q_INVOKABLE int getCurrentIntensityValuesIndex() const;

private slots:
    void updateSensorReading(float lux);

signals:
    void intensityValuesUpdated(int index, float value);
    void currentIntensityValuesIndexChanged(int newIndex);

    void concentrationCoefficientChanged();
    void concentrationMultiplierChanged();

    void rSquaredChanged();
    void yInterceptChanged();
    void slopeChanged();
    void percentEfficiencyChanged();
    void summaryChanged();
};
