#pragma once

#include <QObject>
#include <QTimer>

class SliderHandler : public QObject
{
    Q_OBJECT

private:
    QTimer* m_updateTimer;
    int m_currentValue;

public:
    SliderHandler(QObject* parent = nullptr);

public slots:
    void changeSliderValue(int currValue);
    void onUpdateTimer();

signals:
    void ledIntensityRequested(int currValue);
};
