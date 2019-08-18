#ifndef COUNTER_H
#define COUNTER_H

#include <QObject>

class Counter : public QObject
{
    Q_OBJECT

public:
    Counter();

    int value() const;
    void reset();

public slots:
    void increment();

signals:
    void valueChanged(const int);

private:
    int m_value;
};

#endif // COUNTER_H
