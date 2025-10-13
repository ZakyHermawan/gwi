#include "StateManager.hpp"

#include <QString>

void StateManager::changeCurrentState(const QString &buttonName)
{
    if(buttonName == "Setup")
    {
        m_buttonState = ButtonStates::SETUP;
    }
    else if(buttonName == "Run")
    {
        m_buttonState = ButtonStates::RUN;
    }
    else if(buttonName == "Analysis")
    {
        m_buttonState = ButtonStates::ANALYSIS;
    }
    else if(buttonName == "Amplification Plot")
    {
        m_buttonState = ButtonStates::AMPLIFICATION_PLOT;
    }
    else if(buttonName == "Standard Curve")
    {
        m_buttonState = ButtonStates::STANDARD_CURVE;
    }
    else if(buttonName == "Raw Data")
    {
        m_buttonState = ButtonStates::RAW_DATA;
    }
    else if(buttonName == "Summary")
    {
        m_buttonState = ButtonStates::END;
    }
}
