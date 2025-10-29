#include "RunButtonlEventFilter.hpp"

bool RunButtonEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::LeftButton) {
            is_ready = !is_ready;
            qDebug() << is_ready;

            QVariant currentText = watched->property("text");
            QString newText;

            // Determine the next state based on the current text
            if (currentText.isValid() && currentText.toString() == "Run") {
                newText = "Stop";
            } else if (currentText.isValid() && currentText.toString() == "Stop") {
                newText = "Run";
            } else {
                // Fallback or initial state handler
                newText = "Stop";
            }

            // Apply the new text property to the QML object
            if (!newText.isEmpty()) {
                watched->setProperty("text", newText);
            }

        }
    }
    return QObject::eventFilter(watched, event);
}
