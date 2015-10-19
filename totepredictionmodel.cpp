#include "totepredictionmodel.h"
#include <QColor>
#include <QDebug>

TotePredictionModel::TotePredictionModel(QObject *parent) : QAbstractListModel(parent)
{
}

TotePredictionModel::~TotePredictionModel()
{    
}

void TotePredictionModel::setPrediction(TotePrediction *p)
{
    if (m_prediciton == p)
        return;
    m_prediciton = p;
    QObject::connect(m_prediciton, SIGNAL(predictionChanged(int)), SLOT(onPredictionChanged(int)));
    QObject::connect(m_prediciton, SIGNAL(eventsCountChanged()), SLOT(onEventsCountChanged()));
    this->beginResetModel();
    this->endResetModel();
}

void TotePredictionModel::setAccuracy(QList<QList<double> > acc)
{
    m_acc = acc;
    this->beginResetModel();
    this->endResetModel();
}

int TotePredictionModel::rowCount(const QModelIndex &parent) const
{
    return m_prediciton->eventsCount();
}

int TotePredictionModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant TotePredictionModel::headerData(int section, Qt::Orientation orientation, int role) const
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
    return QVariant();
}

QVariant TotePredictionModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() || index.row() < 0 || index.row() >= m_prediciton->eventsCount() )
        return QVariant();
    int row = index.row();
    int col = index.column();

    if (Qt::BackgroundColorRole == role && m_acc.count() == m_prediciton->eventsCount()) {
        double pval = 0;
        if (0 == col )
            pval = (double) m_prediciton->at(row).home();
        if (1 == col)
            pval = (double) m_prediciton->at(row).draw();
        if (2 == col)
            pval = (double) m_prediciton->at(row).guest();
        double acc = qAbs(pval - m_acc.at(row).at(col));

        if ( 1 <= acc && acc < 2)
            return QColor(Qt::green);
        if ( 2 <= acc && acc < 3)
            return QColor(Qt::yellow);
        if ( 3 <= acc && acc < 5)
            return QColor("orange");
        if ( 5 <= acc)
            return QColor(Qt::red);
    }

    if (Qt::ToolTipRole == role && m_acc.count() == m_prediciton->eventsCount()) {
        return m_acc.at(row).at(col);
    }

    if (Qt::TextAlignmentRole == role)
        return Qt::AlignCenter;

    if (Qt::DisplayRole == role || Qt::EditRole == role) {
        if (0 == col)
            return m_prediciton->at(row).home();
        if (1 == col)
            return m_prediciton->at(row).draw();
        if (2 == col)
            return m_prediciton->at(row).guest();
    }

    return QVariant();
}

Qt::ItemFlags TotePredictionModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool TotePredictionModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( !index.isValid() || index.row() < 0 || index.row() >= m_prediciton->eventsCount() )
        return false;
    int row = index.row();
    int col = index.column();

    if(Qt::EditRole == role) {
        if (0 == col)
            (*m_prediciton)[row].setHome(value.toInt());
        if (1 == col)
            (*m_prediciton)[row].setDraw(value.toInt());
        if (2 == col)
            (*m_prediciton)[row].setGuest(value.toInt());

        if (0 <= col || col <= 2)
            emit dataChanged(index, index);
    }

    return true;
}

void TotePredictionModel::onPredictionChanged(int i)
{
    emit dataChanged(index(i, 0), this->index(i, 2));
}

void TotePredictionModel::onEventsCountChanged()
{
    beginResetModel();
    endResetModel();
}

