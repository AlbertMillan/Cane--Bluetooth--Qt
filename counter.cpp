#include "counter.h"

Counter::Counter()
{
    reset();
}

int Counter::value() const
{
    return m_value;
}

void Counter::reset()
{
    m_value = 0;
}

void Counter::increment()
{
    m_value++;
    emit valueChanged(m_value);
}


