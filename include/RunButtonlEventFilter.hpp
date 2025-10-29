#pragma once

#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>

class RunButtonEventFilter : public QObject
{
    Q_OBJECT
    bool is_ready{1};

public:
    explicit RunButtonEventFilter(QObject *parent = nullptr) : QObject(parent) {}

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};
