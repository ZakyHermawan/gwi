#include "SliderHandler.hpp"

SliderHandler::SliderHandler(QSharedPointer<DataManager> dataManager, QObject* parent)
    : m_dataManager(dataManager), QObject(parent), m_currentValue{0}
{
    setInitialSliderValue(dataManager->getInitialLedIntensityValue());

    // Timer for delayed operations (avoiding too frequent PWM updates)
    m_updateTimer = new QTimer(this);
    m_updateTimer->setSingleShot(true);
    m_updateTimer->setInterval(100); // 100ms delay for PWM
    connect(m_updateTimer, &QTimer::timeout, this, &SliderHandler::onUpdateTimer);
}

int SliderHandler::getCurrentValue()
{
    return m_currentValue;
}

void SliderHandler::changeSliderValue(int currValue)
{
    m_currentValue = currValue;
    m_dataManager->setInitialLedIntensityValue(currValue);
    emit m_dataManager->ledIntensityChanged();
    m_updateTimer->start();
}

void SliderHandler::onUpdateTimer()
{
    // Emit signal to hardware controller
    emit ledIntensityRequested(m_currentValue);
}

void SliderHandler::setInitialSliderValue(int value)
{
    m_currentValue = value;
}
