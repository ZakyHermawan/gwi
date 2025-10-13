#pragma once

#include <QObject>
#include <QTimer>

class SliderHandler : public QObject
{
    Q_OBJECT

    QTimer* m_updateTimer;
    int m_currentValue;

public:
    SliderHandler(int currentValue, QObject* parent = nullptr);

public slots:
    int getCurrentValue() const;
    void changeSliderValue(int currValue);
    void onUpdateTimer();

signals:
    void ledIntensityRequested(int currValue);
};
