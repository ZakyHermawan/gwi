#include <QDebug>
#include <QThread>

#include <cmath>

#ifdef HAVE_WIRINGPI
#include <wiringPi.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#endif

#include "HardwareController.hpp"

/**
 * Constructor : Initialize ledPin and other PWM parameters
 * @param <int> ledPin BCM pin of PWM0 (use `gpio readall`)
 *
 */
HardwareController::HardwareController(uint8_t sensorAddr, int adapter, int ledPin, QObject* parent)
    : QObject(parent)
    , m_sensorAddr(sensorAddr)
    , m_i2cAdapter(adapter)
    , m_ledPin(ledPin)
    , m_ledClockDivisor(640)
    , m_ledPwmRange(101)    // To accomodate for 0-100 integer value range from the slider
    , m_currentIntensity(0)
    , m_i2cFd(-1)
    , m_pcrCycle(0)
    , m_isInitialized(false)
{

#ifdef HAVE_WIRINGPI
    char filename[16];
    snprintf(filename, sizeof(filename), "/dev/i2c-%d", m_i2cAdapter);
    
    m_i2cFd = open(filename, O_RDWR);
    if (m_i2cFd < 0) {
        qFatal("HardwareController: Failed to open I2C bus %s. Aborting...", filename);
    }
#endif

    m_sensorTimer = new QTimer(this);

    /**
     * Sensor reads every (...) ms
     * This implementation does not yet expect DMF control signals,
     * which would make sensor timer intervals irrelevant to the application 
     */
    m_sensorTimer->setInterval(2000); 
    connect(m_sensorTimer, &QTimer::timeout, this, &HardwareController::onSensorTimer);
}

/**
 * Destructor : Closes I2C file and turns LED down upon program exit
 *
 */
HardwareController::~HardwareController()
{
#ifdef HAVE_WIRINGPI
    if (m_i2cFd >= 0) {
        close(m_i2cFd);
    }
    
    if (m_isInitialized) {
        writeLedPwm(0);
    }
#endif
}

#ifdef HAVE_WIRINGPI
/**
 * Public Slot : Calls all hardware initializer methods
 * @return <bool> true if all initializers successfully executed, false otherwise
 *
 */
bool HardwareController::begin()
{
    //QMutexLocker locker(&m_hardwareMutex);    // Currently unused
    
    qDebug() << "HardwareController: Initializing...";
    
    if (!beginWiringPi()) {
        emit errorOccurred("Failed to initialize WiringPi");
        return false;
    }
    
    if (!beginLedPwm()) {
        emit errorOccurred("Failed to initialize LED");
        return false;
    }
    
    if (!beginSensor()) {
        emit errorOccurred("Failed to initialize sensor");
        return false;
    }
    
    m_isInitialized = true;
    emit hardwareInitialized(true);
    qDebug() << "HardwareController: Initialization complete";
    
    return true;
}

/**
 * Private Method : Initializes wiringPi
 * @return <bool> true if initializing successfully executed, false otherwise
 *
 */
bool HardwareController::beginWiringPi()
{
    if (wiringPiSetupPinType(WPI_PIN_BCM) == -1) {
        qDebug() << "HardwareController: Failed to initialize wiringPi";
        return false;
    }
    return true;
}

/**
 * Private Method : Initializes LED and PWM
 * @return <bool> true if initializing successfully executed
 *
 */
bool HardwareController::beginLedPwm()
{
    pinMode(m_ledPin, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(m_ledPwmRange);
    pwmSetClock(m_ledClockDivisor);
    
    writeLedPwm(0);
    
    qDebug() << "HardwareController: LED initialized on pin" << m_ledPin;
    return true;
}

/**
 * Private Method : Initializes BH1750 sensor and I2C bus
 * @return <bool> true if initializing successfully executed, false otherwise
 *
 */
bool HardwareController::beginSensor()
{
    if (ioctl(m_i2cFd, I2C_SLAVE, m_sensorAddr) < 0) {
        qDebug() << "HardwareController: Failed to connect to sensor at address" << Qt::hex << m_sensorAddr;
        close(m_i2cFd);
        m_i2cFd = -1;
        return false;
    }
    
    qDebug() << "HardwareController: Sensor initialized on I2C adapter" << m_i2cAdapter;
    return true;
}
#else
bool HardwareController::begin()
{
    qDebug() << "HardwareController: Initializing...";

    m_i2cFd = 1;

    m_isInitialized = true;
    m_pcrCycle = 0;
    emit hardwareInitialized(true);
    qDebug() << "HardwareController: Initialization complete";

    return true;
}
#endif

#ifdef HAVE_WIRINGPI
void HardwareController::setLEDIntensity(int intensity)
{
    if (!m_isInitialized) {
        qDebug() << "HardwareController: Hardware not initialized";
        return;
    }
    
    //QMutexLocker locker(&m_hardwareMutex);    // Currently unused
    writeLedPwm(intensity);
    m_currentIntensity = intensity;
    
    emit ledIntensityChanged(intensity);    // Currently unused
    qDebug() << "HardwareController: LED intensity set to" << m_currentIntensity << "%";
}

void HardwareController::writeLedPwm(int intensity)
{
    pwmWrite(m_ledPin, intensity);
}
#else
void HardwareController::setLEDIntensity(int intensity)
{
    m_currentIntensity = intensity;
}
#endif

void HardwareController::startSensorReading()
{
    if (m_isInitialized) {
        m_sensorTimer->start();
        qDebug() << "HardwareController: Started sensor reading";
    }
}

void HardwareController::stopSensorReading()
{
    m_sensorTimer->stop();
    qDebug() << "HardwareController: Stopped sensor reading";
}

void HardwareController::onSensorTimer()
{
    if (m_pcrCycle++ < 31) {
            performSensorReading();
            qDebug() << "HardwareController: pcrCycle:" << m_pcrCycle;
    }
    else stopSensorReading();
}

#ifdef HAVE_WIRINGPI
void HardwareController::performSensorReading()
{
    if (!m_isInitialized || m_i2cFd < 0) {
        return;
    }
    
    //QMutexLocker locker(&m_hardwareMutex);    // Currently unused
    
    if (!writeToSensor(ONETIME_H_RES_MODE_2)) {
        emit errorOccurred("Failed to write to sensor");
        return;
    }
    
    // -- Wait for measurement (120ms typical, 180ms max)
    QThread::msleep(180);
    
    // -- Read sensor data
    float lux = readLuxFromSensor();
    if (lux >= 0) {
        emit sensorDataReady(lux);
    }
}

bool HardwareController::writeToSensor(uint8_t mode)
{
    if (write(m_i2cFd, &mode, 1) != 1) {
        qDebug() << "HardwareController: Failed to write to sensor";
        return false;
    }
    return true;
}

float HardwareController::readLuxFromSensor()
{
    unsigned char data[2];
    
    if (read(m_i2cFd, data, 2) != 2) {
        qDebug() << "HardwareController: Failed to read from sensor";
        return -1.0f;
    }
    
    uint16_t raw = (data[0] << 8) | data[1];
    float lux = raw / 1.2f;
    
    qDebug() << "HardwareController: Light detected:" << lux << "lx";
    return lux;
}
#else
void HardwareController::performSensorReading()
{
    if (!m_isInitialized || m_i2cFd < 0) {
        return;
    }
    
    QThread::msleep(180);
    
    float lux;
    if (m_pcrCycle <= 24) lux = pow(2, m_pcrCycle) * 0.00001;
    else lux = 187 - pow(0.5, (m_pcrCycle - 28));
    if (lux >= 0) {
        emit sensorDataReady(lux);
    }
}
#endif
