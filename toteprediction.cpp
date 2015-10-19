#include "toteprediction.h"
#include <QDataStream>
#include <QDebug>

TotePrediction::TotePrediction(int eventscount, QObject *parent) : QObject(parent)
{
    this->resetEvents(eventscount);
    this->setMinDraw(0);
    this->setMinGuest(0);
    this->setMinHome(0);
    this->setMinFavorite(0);
    this->setMinNeutral(0);
    this->setMinOutsider(0);

    this->setMaxDraw(15);
    this->setMaxGuest(15);
    this->setMaxHome(15);
    this->setMaxFavorite(15);
    this->setMaxNeutral(15);
    this->setMaxOutsider(15);
}

TotePrediction::~TotePrediction()
{
}

int TotePrediction::eventsCount() const
{
    return m_eventpredictions.count();
}

int TotePrediction::minHome() const
{
    return m_minhome;
}

int TotePrediction::maxHome() const
{
    return m_maxhome;
}

int TotePrediction::minDraw() const
{
    return m_mindraw;
}

int TotePrediction::maxDraw() const
{
    return m_maxdraw;
}

int TotePrediction::minGuest() const
{
    return m_minguest;
}

int TotePrediction::maxGuest() const
{
    return m_maxguest;
}

int TotePrediction::minFavorite() const
{
    return m_minfavorite;
}

int TotePrediction::maxFavorite() const
{
    return m_maxfavorite;
}

int TotePrediction::minOutsider() const
{
    return m_minoutsider;
}

int TotePrediction::maxOutsider() const
{
    return m_maxoutsider;
}

int TotePrediction::minNeutral() const
{
    return m_minneutral;
}

int TotePrediction::maxNeutral() const
{
    return m_maxneutral;
}

void TotePrediction::setEventsCount(int n)
{
    int m = n;
    if (0 > n)
        m = 0;
    this->resetEvents(m);
}

void TotePrediction::setMinHome(int n)
{
    m_minhome = n;
}

void TotePrediction::setMaxHome(int n)
{
    m_maxhome = n;
}

void TotePrediction::setMinDraw(int n)
{
    m_mindraw = n;
}

void TotePrediction::setMaxDraw(int n)
{
    m_maxdraw = n;
}

void TotePrediction::setMinGuest(int n)
{
    m_minguest = n;
}

void TotePrediction::setMaxGuest(int n)
{
    m_maxguest = n;
}

void TotePrediction::setMinFavorite(int n)
{
    m_minfavorite = n;
}

void TotePrediction::setMaxFavorite(int n)
{
    m_maxfavorite = n;
}

void TotePrediction::setMinOutsider(int n)
{
    m_minoutsider = n;
}

void TotePrediction::setMaxOutsider(int n)
{
    m_maxoutsider = n;
}

void TotePrediction::setMinNeutral(int n)
{
    m_minneutral = n;
}

void TotePrediction::setMaxNeutral(int n)
{
    m_maxneutral = n;
}

const EventPrediction &TotePrediction::at(int i) const
{
    return *m_eventpredictions.at(i);
}

EventPrediction &TotePrediction::operator[](int i)
{
    return *m_eventpredictions[i];
}

void TotePrediction::onChanged()
{
    EventPrediction *d = dynamic_cast<EventPrediction *>(sender());
    for(int i = 0; i < m_eventpredictions.count(); i++)
        if (d == m_eventpredictions.at(i))
            emit predictionChanged(i);
}

void TotePrediction::resetEvents(int n)
{
    int currentcount = m_eventpredictions.count();
    if (n == currentcount)
        return;
    if (n > currentcount) {
        for(int i = currentcount; i < n; i++) {
            EventPrediction *ep = new EventPrediction(this);
            QObject::connect(ep, SIGNAL(changed()), SLOT(onChanged()));
            m_eventpredictions.append(ep);
        }
    } else
        for(int i = currentcount; n < i; i--) {
            delete m_eventpredictions.last();
            m_eventpredictions.removeLast();
        }

    emit this->eventsCountChanged();
}

QDataStream &operator <<(QDataStream &out, const TotePrediction &tp)
{
    out << tp.eventsCount() << tp.minHome() << tp.maxHome() << tp.minDraw() << tp.maxDraw()
        << tp.minGuest() << tp.maxGuest() << tp.minFavorite() << tp.maxFavorite() << tp.minNeutral() << tp.maxNeutral()
        << tp.minOutsider() << tp.maxOutsider();
    for(int i = 0; i < tp.eventsCount(); i++)
        out << tp.at(i);
    return out;
}

QDataStream &operator >>(QDataStream &in, TotePrediction &tp)
{
    int minh, maxh, mind, maxd, ming, maxg, minf, maxf, minn, maxn, mino, maxo, count;
    in >> count >> minh >> maxh >> mind >> maxd >> ming >> maxg >> minf >> maxf >> minn >> maxn >> mino >> maxo;
    tp.setMinHome(minh);
    tp.setMaxHome(maxh);
    tp.setMinDraw(mind);
    tp.setMaxDraw(maxd);
    tp.setMinGuest(ming);
    tp.setMaxGuest(maxg);
    tp.setMinFavorite(minf);
    tp.setMaxFavorite(maxf);
    tp.setMinNeutral(minn);
    tp.setMaxNeutral(maxn);
    tp.setMinOutsider(mino);
    tp.setMaxOutsider(maxo);
    tp.setEventsCount(count);
    for(int i = 0; i < tp.eventsCount(); i++)
        in >> tp[i];
    return in;
}
