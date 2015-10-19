#ifndef EVENTPREDICTION_H
#define EVENTPREDICTION_H

#include <QObject>

class EventPrediction : public QObject
{
    Q_OBJECT
public:
    explicit EventPrediction(QObject *parent = 0);
    explicit EventPrediction(int h, int d = 0, QObject *parent = 0);
    ~EventPrediction();
    int home() const;
    int draw() const;
    int guest() const;
    int favorite() const;
    int neutral() const;
    int outsider() const;
    void set(int h, int d);
    void setHome(int h);
    void setDraw(int d);
    void setGuest(int g);

signals:
    void changed();

private:
    int m_home;
    int m_draw;
    int m_guest;
};

QDataStream &operator <<(QDataStream &out, const EventPrediction &ep);
QDataStream &operator >>(QDataStream &in, EventPrediction &ep);

#endif // EVENTPREDICTION_H
