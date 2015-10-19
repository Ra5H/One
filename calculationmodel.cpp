#include "calculationmodel.h"

CalculationModel::CalculationModel(QObject *parent) : QAbstractListModel(parent)
{

}

CalculationModel::~CalculationModel()
{

}

void CalculationModel::setResult(const QList<int> &rl)
{
    m_list = rl;
    for(int i = 0; i < m_list.count(); i++)
        m_keflist.append(0);
    if (14 == m_list.count() - 1)
        m_cost = 30;
    else
        m_cost = 50;
    this->beginResetModel();
    this->endResetModel();
    this->calcSum();
}

int CalculationModel::rowCount(const QModelIndex &parent) const
{
    return m_list.count() - 1;
}

int CalculationModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant CalculationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (Qt::DisplayRole != role)
        return QVariant();
    if (Qt::Vertical == orientation)
        return m_list.count() - (section + 1);
    if (section == 0)
        return "Угаданно";
    if (section == 1)
        return "Коэф";
    if (section == 2)
        return "Выгрыш";
    return QVariant();
}

QVariant CalculationModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() || index.row() < 0 || index.row() >= m_list.count() )
        return QVariant();
    int row = index.row();
    int col = index.column();

    if (Qt::TextAlignmentRole == role)
        return Qt::AlignCenter + Qt::AlignVCenter;

    if (Qt::DisplayRole == role) {
        if (0 == col)
            return m_list.at(m_list.count() - (row + 1));
        if (1 == col)
            return m_keflist.at(row);
        if (2 == col)
            return m_list.at(m_list.count() - (row + 1)) * m_keflist.at(row) * m_cost;
    }

    return QVariant();
}

Qt::ItemFlags CalculationModel::flags(const QModelIndex &index) const
{
    if (!index.isValid() || 0 == index.column() || 2 == index.column())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool CalculationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( !index.isValid() || index.row() < 0 || index.row() >= m_list.count() )
        return false;
    int row = index.row();
    int col = index.column();

    if(Qt::EditRole == role) {
        if (1 == col)
            m_keflist[row] = value.toFloat();

        if (0 <= col || col <= 2) {
            emit dataChanged(index, index);
        }
        emit this->calcSum();
    }

    return true;
}

void CalculationModel::calcSum()
{
    quint64 sum = 0;
    for (int i = 0; i < m_list.count(); i++)
        sum += m_list.at(m_list.count() - (i + 1)) * m_keflist.at(i) * m_cost;
    emit this->totalWon(sum);
}

