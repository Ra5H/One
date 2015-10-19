#ifndef CALCULATIONMODEL_H
#define CALCULATIONMODEL_H

#include <QAbstractListModel>

class CalculationModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CalculationModel(QObject *parent = 0);
    ~CalculationModel();
    void setResult(const QList<int> &rl);

signals:
    void totalWon(const qint64 &sum);

public:
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

public slots:

private slots:

private:
    void calcSum();

private:
    QList<int> m_list;
    QList<float> m_keflist;
    int m_cost;
};

#endif // CALCULATIONMODEL_H
