#ifndef TOTEPREDICTIONMODEL_H
#define TOTEPREDICTIONMODEL_H

#include <QAbstractListModel>
#include <toteprediction.h>

class TotePredictionModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TotePredictionModel(QObject *parent = 0);
    ~TotePredictionModel();
    void setPrediction(TotePrediction *p);
    void setAccuracy(QList <QList<double> > acc);

signals:

public:
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

public slots:

private slots:
    void onPredictionChanged(int i);
    void onEventsCountChanged();

private:

private:
    TotePrediction* m_prediciton;
    QList <QList<double> > m_acc;
};

#endif // TOTEPREDICTIONMODEL_H
