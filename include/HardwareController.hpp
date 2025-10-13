#pragma once

#include <QObject>
#include <QTimer>
//#include <QMutex> // Currently unused

class HardwareController : public QObject {

    Q_OBJECT

    // -- PWM-LED control
    const int m_ledPin;
    int m_ledClockDivisor;  // Defaulted to 32 --> 640
    int m_ledPwmRange;          // Defaulted to 1024 --> 100
    int m_currentIntensity;

    // -- BH1750 sensor control
    int m_i2cAdapter;
    int m_i2cFd;
    uint8_t m_sensorAddr;
    int m_pcrCycle;

    // -- Threading
    QTimer* m_sensorTimer;
    //QMutex m_hardwareMutex;   // Currently unused
    bool m_isInitialized;

#ifdef HAVE_WIRINGPI
    /**
     * Hardware methods adopted from
     * https://github.com/arkandzprogaming/pcr-instrument-mproc.git
     *  
     */
    bool beginWiringPi();
    bool beginLedPwm();
    bool beginSensor();
    
    // -- LED control method
    void writeLedPwm(int intensity);

    // -- Sensor iteraction methods
    bool writeToSensor(uint8_t mode);
#endif
        
public:
    explicit HardwareController(uint8_t sensorAddr = 0x23, int adapter = 1, int ledPin = 18, QObject* parent = nullptr);
    ~HardwareController();

    // -- BH1750 sensor operation modes
    enum SensorMode : uint8_t {
        CONTINUOUSLY_H_RES_MODE = 0x10,
        CONTINUOUSLY_H_RES_MODE_2 = 0x11,
        CONTINUOUSLY_L_RES_MODE = 0x13,
        ONETIME_H_RES_MODE = 0x20,
        ONETIME_H_RES_MODE_2 = 0x21,
        ONETIME_L_RES_MODE = 0x23
    };

public slots:
    bool begin();
    void setLEDIntensity(int intensity);
    void startSensorReading();
    void stopSensorReading();
    void performSensorReading();

private slots:
#ifdef HAVE_WIRINGPI
    float readLuxFromSensor();
#endif
    void onSensorTimer();

signals:
    void hardwareInitialized(bool success);
    void ledIntensityChanged(int intensity);
    void sensorDataReady(float lux);
#ifdef HAVE_WIRINGPI
    void errorOccurred(const QString& error);
#endif
};
