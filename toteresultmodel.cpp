#include "toteresultmodel.h"
#include <QDebug>

ToteResultModel::ToteResultModel(int c, QObject *parent) : QAbstractListModel(parent)
{
    this->setCount(c);
}

ToteResultModel::~ToteResultModel()
{
}

bool ToteResultModel::isReady() const
{
    for(int i = 0; i < m_list.count(); i++)
        if (m_list.at(i) < 0)
            return false;
    return true;
}

QList<int> ToteResultModel::result() const
{
    return m_list;
}

void ToteResultModel::setResult(const QList<int> &rl)
{
    m_list =rl;
    this->beginResetModel();
    this->endResetModel();
}

void ToteResultModel::setCount(int n)
{
    m_list.clear();
    for(int i = 0; i < n; i++)
        m_list.append(-1);
    this->beginResetModel();
    this->endResetModel();
}

int ToteResultModel::rowCount(const QModelIndex &parent) const
{
    return m_list.count();
}

int ToteResultModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant ToteResultModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole != role)
        return QVariant();
    if (Qt::Vertical == orientation)
        return section + 1;
    if (section == 0)
        return "1";
    if (section == 1)
        return "X";
    if (section == 2)
        return "2";
}

QVariant ToteResultModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() || index.row() < 0 || index.row() >= m_list.count() )
        return QVariant();
    int row = index.row();
    int col = index.column();

    if (Qt::TextAlignmentRole == role)
        return Qt::AlignCenter + Qt::AlignVCenter;

    if (Qt::CheckStateRole == role) {
        int v = m_list.at(row);
        if (v == 0 && col == 1)
            return Qt::Checked;
        else if (v == 1 && col == 0)
            return Qt::Checked;
        else if (v == 2 && col == 2)
            return Qt::Checked;
        else
            return Qt::Unchecked;
    }

    return QVariant();
}

Qt::ItemFlags ToteResultModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

bool ToteResultModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( !index.isValid() || index.row() < 0 || index.row() >= m_list.count() )
        return false;
    int row = index.row();
    int col = index.column();

    if (Qt::CheckStateRole == role) {
        if (Qt::Checked == value) {
            if (0 == col)
                m_list[row] = 1;
            if (1 == col)
                m_list[row] = 0;
            if (2 == col)
                m_list[row] = 2;
        } else
            m_list[row] = -1;
    }

    this->beginResetModel();
    this->endResetModel();

//    if (this->isReady())
        emit this->ready();

    return true;
}

