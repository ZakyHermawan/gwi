#pragma once

#include "fkYAML.hpp"

#include <QObject>
#include <QList>
#include <QPair>
#include <QMap>

#include <string>

class DataManager : public QObject
{
    Q_OBJECT
public:
    // List of intensity values for amplification plot and raw data
    QList<float> m_intensityValues;
    int m_currentIntensityValuesIndex;

    // Current intensity in setup
    int m_ledIntensity;
    // Max cycle in setup
    int m_maxCycle;
    // Timestamp when the experiment is last saved
    std::string m_lastSaved;
    // Ct value
    int m_cycleThreshold;
    // Minimum light intensity so note the sample as "amplified"
    double m_intensityThreshold;

    // experiment names or key of m_experiments always have ".yml"
    QMap<QString, fkyaml::node> m_experiments;
    fkyaml::node m_currentExperiment;
    QString m_currentExperimentName;
    QList<QString> m_experimentNames;

    // Use QString for display on setup,
    // convert to float later for processing
    QString m_concentrationCoefficient;
    float m_concentrationMultiplier;

    // Standard curve data
    double m_rSquared;
    double m_yIntercept;
    double m_slope;
    double m_percentEfficiency;
    QString m_summary;
    QList<QPair<double, int>> m_xyLogStandardCurve;

    Q_PROPERTY(int ledIntensity
        MEMBER m_ledIntensity
        NOTIFY ledIntensityChanged)

    Q_PROPERTY(double intensityThreshold
        MEMBER m_intensityThreshold
        NOTIFY intensityThresholdChanged)

    Q_PROPERTY(int cycleThreshold
        MEMBER m_cycleThreshold
        NOTIFY cycleThresholdChanged)

    Q_PROPERTY(int maxCycle
        MEMBER m_maxCycle
        NOTIFY maxCycleChanged)

    Q_PROPERTY(QString currentExperimentName
        MEMBER m_currentExperimentName
        NOTIFY currentExperimentNameChanged)

    Q_PROPERTY(QString concentrationCoefficient
        MEMBER m_concentrationCoefficient
        NOTIFY concentrationCoefficientChanged)

    Q_PROPERTY(float concentrationMultiplier
        MEMBER m_concentrationMultiplier
        NOTIFY concentrationMultiplierChanged)

    Q_PROPERTY(double rSquared
        MEMBER m_rSquared
        NOTIFY rSquaredChanged)

    Q_PROPERTY(double yIntercept
        MEMBER m_yIntercept
        NOTIFY yInterceptChanged)

    Q_PROPERTY(double slope
        MEMBER m_slope
        NOTIFY slopeChanged)

    Q_PROPERTY(double percentEfficiency
        MEMBER m_percentEfficiency
        NOTIFY percentEfficiencyChanged)

    Q_PROPERTY(QString summary
        MEMBER m_summary
        NOTIFY summaryChanged)

    void updateLedIntensity();
    void updateMaxCycle();
    void updateIntensityThreshold();
    void updateCycleThreshold();
    void updateConcentrationCoefficient();
    void updateConcentrationMultiplier();
    void updateXYStandardCurve();
    void createExperimentFromTemplate(const QString& newName);

public:
    DataManager() = default;
    DataManager(QMap<QString, fkyaml::node>& experiments);
    QList<float>& getIntensityValuesList();
    QList<QPair<double, int>>& getXyLogStandardCurve();
    QList<QString>& getExperimentNames();
    void save_data();
    QString getCurrTimeStampStr();
    /*
     * Math Representation:
     *
     * Σ( (x_i - x̄) * (y_i - ȳ) )
     * Slope (m) = --------------------------
     * Σ( (x_i - x̄)² )
     *
     * Intercept (b) = ȳ - (m * x̄)
     *
     * --------------------------------------------------------------------------------------
     * Variables:
     * x̄ (x_bar) = Mean of X
     * ȳ (y_bar) = Mean of Y
     * Σ         = Summation loop
     *
     * Why use formula this?
     * It prevents "Catastrophic Cancellation". By subtracting the mean first,
     * we calculate using small "delta" values rather than massive raw totals.
     * Read: What Every Computer Scientist Should Know About Floating-Point Arithmetic
     * ======================================================================================
     */
    std::tuple<double, double, double>
    simpleLinearRegression(const std::vector<double>& x, const std::vector<double>& y);

    /*
     * Math Representation:
     *
     * Efficiency (%) = ( 10^(-1 / Slope) - 1 ) * 100
     *
     * --------------------------------------------------------------------------------------
     * Derivation Logic:
     * 1. Ideally, PCR product doubles every cycle: N = N0 * 2^Ct
     * 2. In log-log space, this doubling creates a theoretical slope of:
     * m = -1 / log10(2) ≈ -3.3219
     * 3. We reverse this to find the actual amplification factor:
     * Factor = 10^(-1 / Slope)
     * (e.g., Factor 2.0 = 100% efficient, Factor 1.9 = 90% efficient)
     *
     * --------------------------------------------------------------------------------------
     * 1. Ideal Efficiency (100%):
     * - A slope of -3.322 means that the PCR has an efficiency of 1, or 100%.
     * - The amount of PCR product doubles during each cycle.
     *
     * 2. Low Efficiency (< 100%):
     * - A slope of less than -3.322 (e.g., -3.8) indicates a PCR efficiency < 1.
     * - Generally, most amplification reactions do not reach 100% efficiency due
     * to experimental limitations.
     *
     * 3. High Efficiency (> 100%):
     * - A slope greater than -3.322 (e.g., -3.0) indicates a PCR efficiency that
     * appears to be greater than 100%.
     * - This can occur when values are measured in the nonlinear phase of the
     * reaction, or it can indicate the presence of inhibitors in the reaction.
     *
     * Source: https://www.qiagen.com/us/knowledge-and-support/knowledge-hub/bench-guide/pcr/commonly-used-terms-in-pcr/commonly-used-terms-in-pcr
     * --------------------------------------------------------------------------------------
     * Edge Case Safety:
     * - If Slope is 0 (flat line): Efficiency approaches Infinity (Undefined).
     * - If Slope is positive: Input data is likely inverted or garbage.
     * ======================================================================================
     */
    double calculatePCREfficiency(double slope);

    void setCycleThreshold();
    int getInitialLedIntensityValue();
    void setInitialLedIntensityValue(int ledIntensityValue);
    void removeExperiment(const QString experimentName);
    void addExperiment(const QString experimentName);

public slots:
    Q_INVOKABLE void setMaxCycle(int maxCycle);
    Q_INVOKABLE void updateCurrentExperiment();
    // set every private members to the value of current experiment data
    Q_INVOKABLE void loadCurrentExperiment();

    void calculateStandardCurve();

    void resetIntensityValues();
    void resetStandardCurveData();

    Q_INVOKABLE float getIntensityValueByIndex(int index);
    void addSensorReading(float lux);
    void setIntensityValuesSize(int size);
    Q_INVOKABLE int getIntensityValuesSize() const;
    Q_INVOKABLE int getStandardCurveDataSize() const;

    Q_INVOKABLE float getConcentrationMultiplier() const;
    Q_INVOKABLE void setConcentrationMultiplier(float multiplier);

    Q_INVOKABLE int getCurrentIntensityValuesIndex() const;
    Q_INVOKABLE void updateCurrentExperimentName(QString& currentExperimentName);
    void resetCurrentExperiment();

private slots:
    void updateSensorReading(float lux);

signals:
    void intensityValuesUpdated(int index, float value);
    void xyLogStandardCurveUpdated();
    void currentIntensityValuesIndexChanged(int newIndex);
    void maxCycleChanged();

    void ledIntensityChanged();
    void intensityThresholdChanged();
    void cycleThresholdChanged();
    void currentExperimentNameChanged();

    void concentrationCoefficientChanged();
    void concentrationMultiplierChanged();

    void rSquaredChanged();
    void yInterceptChanged();
    void slopeChanged();
    void percentEfficiencyChanged();
    void summaryChanged();
    void standardCurveBoundsChanged();
};
