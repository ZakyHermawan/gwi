#include "SliderHandler.hpp"

#include <QDebug>

SliderHandler::SliderHandler(QObject* parent) 
    : QObject(parent), m_currentValue(0)
{
    // -- Timer for delayed operations (avoiding too frequent PWM updates)
    m_updateTimer = new QTimer(this);
    m_updateTimer->setSingleShot(true);
    m_updateTimer->setInterval(100); // 100ms delay for PWM
    connect(m_updateTimer, &QTimer::timeout, this, &SliderHandler::onUpdateTimer);
}

void SliderHandler::changeSliderValue(int currValue)
{
    qDebug() << "Current slider value: " << currValue;
    m_currentValue = currValue;
    
    m_updateTimer->start();
}

void SliderHandler::onUpdateTimer()
{
    // Emit signal to hardware controller
    emit ledIntensityRequested(m_currentValue);
}
