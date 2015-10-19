#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <totepredictionmodel.h>
#include <toteresultmodel.h>
#include <calculationmodel.h>

namespace Ui {
class MainWindow;
}

typedef QList<int> Coupon;
typedef QList<Coupon> Packet;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onCountCahnged();
    void generate();
    void updateTotePredictionView();
    void updateTotePrediction();
    void onSavePrediction();
    void onLoadPrediction();
    void onLoadPacket();
    void calculateResult();
    void onWonSum(const qint64 &sum);
    void onImport();

private:
    int getRandomResult(int t1p, int txp, int t2p);
    void maxOffset(const TotePrediction &tp, const Packet &pack, int &event, int &result);
    void calcAccuracy();
    int valueCount(int value, const Coupon &c) const;
    int allocationCount(int lev, const Coupon &c, const TotePrediction &tp);
    int matchsCount(const Coupon &c1, const Coupon &c2) const;

private:
    Ui::MainWindow *ui;
    TotePredictionModel m_model;
    ToteResultModel m_resultmodel;
    CalculationModel m_calcmodel;
    Packet m_packet;
    Packet m_import;
    TotePrediction m_tp;
    QList<int> m_packetresult;
    int m_ec;
};

#endif // MAINWINDOW_H
