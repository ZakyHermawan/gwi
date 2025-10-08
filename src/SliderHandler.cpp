#include "SliderHandler.hpp"

#include <QDebug>
void SliderHandler::changeSliderValue(int currValue)
{
    qDebug() << "Current slider value: " << currValue;
}
