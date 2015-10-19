#include "eventprediction.h"
#include <QDataStream>

EventPrediction::EventPrediction(QObject *parent) : QObject(parent)
{
    m_home = 39;
    m_draw = 27;
    m_guest = 34;
}

EventPrediction::EventPrediction(int h, int d, QObject *parent) : QObject(parent)
{
    this->set(h, d);
}

EventPrediction::~EventPrediction()
{
}

int EventPrediction::home() const
{
    return m_home;
}

int EventPrediction::draw() const
{
    return m_draw;
}

int EventPrediction::guest() const
{
    return m_guest;
}

int EventPrediction::favorite() const
{
    int max = qMax(m_home, qMax(m_draw, m_guest));
    if (max == m_draw)
        return 0;
    if (max == m_home)
        return 1;
    return 2;
}

int EventPrediction::neutral() const
{
    int max = this->favorite();
    if (max == 1) {
        if (qMax(m_draw, m_guest) == m_draw)
            return 0;
        else
            return 2;
    }
    if (max == 0) {
        if (qMax(m_home, m_guest) == m_home)
            return 1;
        else
            return 2;
    }
    if (qMax(m_home, m_draw) == m_home)
        return 1;
    return 0;
}

int EventPrediction::outsider() const
{
    int min = qMin(m_home, qMin(m_draw, m_guest));
    if (min == m_draw)
        return 0;
    if (min == m_home)
        return 1;
    return 2;
}

void EventPrediction::set(int h, int d)
{
    if (0 > h)
        m_home = 0;
    else if (100 < m_home)
        m_home = 100;
    else
        m_home = h;

    if (0 > d)
        m_draw = 0;
    else if (100 < m_home + d)
        m_draw = 100 - m_home;
    else
        m_draw = d;

    m_guest = 100 - ( m_home + m_draw );
    emit this->changed();
}

void EventPrediction::setHome(int h)
{
    if (h == m_home)
        return;

    if (0 > h)
        m_home = 0;
    else if (100 < h)
        m_home = 100;
    else
        m_home = h;

    if (100 < m_home + m_guest)
        m_guest = 100 - m_home;

    m_draw = 100 - ( m_home + m_guest );
    emit this->changed();
}

void EventPrediction::setDraw(int d)
{
    if (d == m_draw)
        return;

    if (0 > d)
        m_draw = 0;
    else if (100 < d)
        m_draw = 100;
    else
        m_draw = d;

    if (100 < m_draw + m_home)
        m_home = 100 - m_draw;

    m_guest = 100 - ( m_home + m_draw );
    emit this->changed();
}

void EventPrediction::setGuest(int g)
{
    if (g == m_guest)
        return;

    if (0 > g)
        m_guest = 0;
    else if (100 < g)
        m_guest = 100;
    else
        m_guest = g;

    if (100 < m_guest + m_draw)
        m_draw = 100 - m_guest;

    m_home = 100 - ( m_guest + m_draw );
    emit this->changed();
}

QDataStream &operator <<(QDataStream &out, const EventPrediction &ep)
{
    out << ep.home() << ep.draw();
    return out;
}

QDataStream &operator >>(QDataStream &in, EventPrediction &ep)
{
    int h;
    int d;
    in >> h >> d;
    ep.set(h, d);
    return in;
}
