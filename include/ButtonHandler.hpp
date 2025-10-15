#include <QObject>
#include <QString>

class ButtonHandler : public QObject
{
    Q_OBJECT

public slots:
    void handleButtonClick(const QString &buttonName);
    void saveDataClick();
};
