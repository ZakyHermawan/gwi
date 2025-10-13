#pragma once

#include <QObject>
#include <QString>

enum class ButtonStates
{
    SETUP,
    RUN,
    ANALYSIS,
    AMPLIFICATION_PLOT,
    STANDARD_CURVE,
    RAW_DATA,
    SUMMARY,
    END
};

class StateManager : public QObject
{
    Q_OBJECT

    ButtonStates m_buttonState;

public slots:
    void changeCurrentState(const QString &currState);
};
