#ifndef TOTERESULTMODEL_H
#define TOTERESULTMODEL_H

#include <QAbstractListModel>

class ToteResultModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ToteResultModel(int c = 15, QObject *parent = 0);
    ~ToteResultModel();
    bool isReady() const;
    QList<int> result() const;
    void setResult(const QList<int> &rl);
    void setCount(int n);

signals:
    void ready();

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

private:
    QList<int> m_list;
};
#endif // TOTERESULTMODEL_H
