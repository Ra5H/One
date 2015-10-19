#ifndef TOTEPREDICTION_H
#define TOTEPREDICTION_H

#include <QObject>
#include <eventprediction.h>

class TotePrediction : public QObject
{
    Q_OBJECT
public:
    explicit TotePrediction(int eventscount = 0, QObject *parent = 0);
    ~TotePrediction();

    int eventsCount() const;
    int minHome() const;
    int maxHome() const;
    int minDraw() const;
    int maxDraw() const;
    int minGuest() const;
    int maxGuest() const;
    int minFavorite() const;
    int maxFavorite() const;
    int minOutsider() const;
    int maxOutsider() const;
    int minNeutral() const;
    int maxNeutral() const;

    void setEventsCount(int n);
    void setMinHome(int n);
    void setMaxHome(int n);
    void setMinDraw(int n);
    void setMaxDraw(int n);
    void setMinGuest(int n);
    void setMaxGuest(int n);
    void setMinFavorite(int n);
    void setMaxFavorite(int n);
    void setMinOutsider(int n);
    void setMaxOutsider(int n);
    void setMinNeutral(int n);
    void setMaxNeutral(int n);

    const EventPrediction& at(int i) const;
    EventPrediction& operator[](int i);

signals:
    void predictionChanged(int index);
    void eventsCountChanged();

public slots:
    void onChanged();

private:
    void resetEvents(int n);

private:
    int m_minhome;
    int m_maxhome;
    int m_mindraw;
    int m_maxdraw;
    int m_minguest;
    int m_maxguest;

    int m_minfavorite;
    int m_maxfavorite;
    int m_minoutsider;
    int m_maxoutsider;
    int m_minneutral;
    int m_maxneutral;

    QList<EventPrediction *> m_eventpredictions;
};

QDataStream &operator <<(QDataStream &out, const TotePrediction &tp);
QDataStream &operator >>(QDataStream &in, TotePrediction &tp);

#endif // TOTEPREDICTION_H
