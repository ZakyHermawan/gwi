#include "SliderHandler.hpp"

SliderHandler::SliderHandler(int currentValue, QObject* parent)
    : m_currentValue(currentValue), QObject(parent)
{
    // -- Timer for delayed operations (avoiding too frequent PWM updates)
    m_updateTimer = new QTimer(this);
    m_updateTimer->setSingleShot(true);
    m_updateTimer->setInterval(100); // 100ms delay for PWM
    connect(m_updateTimer, &QTimer::timeout, this, &SliderHandler::onUpdateTimer);
}

int SliderHandler::getCurrentValue() const
{
    return m_currentValue;
}

void SliderHandler::changeSliderValue(int currValue)
{
    m_currentValue = currValue;
    m_updateTimer->start();
}

void SliderHandler::onUpdateTimer()
{
    // Emit signal to hardware controller
    emit ledIntensityRequested(m_currentValue);
}
