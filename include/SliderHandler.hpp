#pragma once

#include <QObject>
#include <QTimer>
#include <QSharedPointer>

#include "DataManager.hpp"

class SliderHandler : public QObject
{
    Q_OBJECT

    QTimer* m_updateTimer;
    int m_currentValue;
    QSharedPointer<DataManager> m_dataManager;

public:
    SliderHandler(QSharedPointer<DataManager> dataManager, QObject* parent = nullptr);
    void setInitialSliderValue(int value);

public slots:
    int getCurrentValue();
    void changeSliderValue(int currValue);
    void onUpdateTimer();

signals:
    void ledIntensityRequested(int currValue);
};
