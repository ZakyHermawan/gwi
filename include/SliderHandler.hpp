#pragma once

#include <QObject>
class SliderHandler : public QObject
{
    Q_OBJECT

public slots:
    void changeSliderValue(int currValue);
};

