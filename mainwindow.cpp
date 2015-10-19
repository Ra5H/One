#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <QFileDialog>
#include <QMessageBox>
#include <totepredictionmodel.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setValue(100);

    QObject::connect(&m_calcmodel, SIGNAL(totalWon(qint64)), SLOT(onWonSum(qint64)));
    QObject::connect(ui->packetButton, SIGNAL(clicked()), SLOT(generate()));
    QObject::connect(ui->impotButton, SIGNAL(clicked()), SLOT(onImport()));
    QObject::connect(ui->savePredictionButton, SIGNAL(clicked()), SLOT(onSavePrediction()));
    QObject::connect(ui->loadPredictionButton, SIGNAL(clicked()), SLOT(onLoadPrediction()));
    QObject::connect(ui->loadPacketButton, SIGNAL(clicked()), SLOT(onLoadPacket()));
    QObject::connect(&m_resultmodel, SIGNAL(ready()), SLOT(calculateResult()));
    QObject::connect(ui->countComboBox, SIGNAL(currentIndexChanged(QString)), SLOT(onCountCahnged()));
    QObject::connect(ui->minhEdit, SIGNAL(textEdited(QString)), SLOT(updateTotePrediction()));
    QObject::connect(ui->mindEdit, SIGNAL(textEdited(QString)), SLOT(updateTotePrediction()));
    QObject::connect(ui->mingEdit, SIGNAL(textEdited(QString)), SLOT(updateTotePrediction()));
    QObject::connect(ui->minfEdit, SIGNAL(textEdited(QString)), SLOT(updateTotePrediction()));
    QObject::connect(ui->minnEdit, SIGNAL(textEdited(QString)), SLOT(updateTotePrediction()));
    QObject::connect(ui->minoEdit, SIGNAL(textEdited(QString)), SLOT(updateTotePrediction()));
    QObject::connect(ui->maxhEdit, SIGNAL(textEdited(QString)), SLOT(updateTotePrediction()));
    QObject::connect(ui->maxdEdit, SIGNAL(textEdited(QString)), SLOT(updateTotePrediction()));
    QObject::connect(ui->maxgEdit, SIGNAL(textEdited(QString)), SLOT(updateTotePrediction()));
    QObject::connect(ui->maxfEdit, SIGNAL(textEdited(QString)), SLOT(updateTotePrediction()));
    QObject::connect(ui->maxnEdit, SIGNAL(textEdited(QString)), SLOT(updateTotePrediction()));
    QObject::connect(ui->maxoEdit, SIGNAL(textEdited(QString)), SLOT(updateTotePrediction()));

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    m_ec = ui->countComboBox->currentText().toInt();
    m_tp.setEventsCount(m_ec);
    m_resultmodel.setCount(m_ec);
    m_model.setPrediction(&m_tp);
    this->updateTotePredictionView();
    ui->predictionTableView->setModel(&m_model);

    ui->resultTableView->setModel(&m_resultmodel);
    ui->resultTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->calculationTableView->setModel(&m_calcmodel);
    ui->calculationTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCountCahnged()
{
    m_ec = ui->countComboBox->currentText().toInt();
    m_tp.setEventsCount(m_ec);
    m_resultmodel.setCount(m_ec);
}

void MainWindow::generate()
{
    int count = ui->countEdit->text().toInt();
    qDebug() << QString("count: %1").arg(count);
    ui->progressBar->setValue(0);
    ui->progressBar->setRange(0, count);
    Packet pack;
    while (pack.count() < count) {
        Coupon k;
        int maxOff;
        int event = -1;
        this->maxOffset(m_tp, pack, event, maxOff);
        for (int j = 0; j < m_tp.eventsCount(); j++) {
            int val = this->getRandomResult(m_tp.at(j).home(), m_tp.at(j).draw(), m_tp.at(j).guest());
            if (event == j)
                val = maxOff;
            k.append(val);
        }
        int c;
        c = this->valueCount(1, k);
        if (c < m_tp.minHome() || c > m_tp.maxHome())
            continue;
        c = this->valueCount(0, k);
        if (c < m_tp.minDraw() || c > m_tp.maxDraw())
            continue;
        c = this->valueCount(2, k);
        if (c < m_tp.minGuest() || c > m_tp.maxGuest())
            continue;
        c = this->allocationCount(1, k, m_tp);
        if (c < m_tp.minFavorite() || c > m_tp.maxFavorite())
            continue;
        c = this->allocationCount(0, k, m_tp);
        if (c < m_tp.minNeutral() || c > m_tp.maxNeutral())
            continue;
        c = this->allocationCount(-1, k, m_tp);
        if (c < m_tp.minOutsider() || c > m_tp.maxOutsider())
            continue;        

        if (pack.contains(k))
            continue;
        if (m_import.contains(k))
            continue;
        pack.append(k);

        ui->progressBar->setValue(pack.count());
    }

    m_packet = pack;
    this->calcAccuracy();
    this->calculateResult();

    QString fileName = QFileDialog::getSaveFileName(this,
        "Выберите файл", QString(getenv("HOME")), tr("Text Files (*.txt)"));

    if (fileName.isNull())
        return;
    if (!QFile::exists(fileName))
        fileName.append(".txt");

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Ошибка", "Неудалось открыть файл");
        return;
    }
    QTextStream stream(&file);

    QString cost;
    if (15 == m_ec)
        cost = "50.00"; // fon
    else
        cost = "30.00"; // city
    for(int i = 0; i < pack.count(); i++) {
        if (15 == m_ec)
            stream << QString("%1; ").arg(cost); // fon
        else
            stream << QString("%1;").arg(cost); // city
        Coupon k = pack.at(i);
        for(int j = 0; j < k.count(); j++) {
            QString val;
            if (0 == k.at(j))
                val = "X";
            else
                val = QString::number(k.at(j));
            if (15 == m_ec) {
                stream << QString("%1-(%2)").arg(QString::number(j+1), val); // fon
                if (j+1 != k.count()) // fon
                    stream << "; ";   // fon
                else                  // fon
                    stream << ".";    // fon
            }
            else
                stream << QString("%1=%2;").arg(QString::number(j+1), val); // city
        }
        stream << endl;
    }    

    file.close();    
}

void MainWindow::updateTotePredictionView()
{
    ui->minhEdit->setText(QString::number(m_tp.minHome()));
    ui->mindEdit->setText(QString::number(m_tp.minDraw()));
    ui->mingEdit->setText(QString::number(m_tp.minGuest()));
    ui->minfEdit->setText(QString::number(m_tp.minFavorite()));
    ui->minnEdit->setText(QString::number(m_tp.minNeutral()));
    ui->minoEdit->setText(QString::number(m_tp.minOutsider()));
    ui->maxhEdit->setText(QString::number(m_tp.maxHome()));
    ui->maxdEdit->setText(QString::number(m_tp.maxDraw()));
    ui->maxgEdit->setText(QString::number(m_tp.maxGuest()));
    ui->maxfEdit->setText(QString::number(m_tp.maxFavorite()));
    ui->maxnEdit->setText(QString::number(m_tp.maxNeutral()));
    ui->maxoEdit->setText(QString::number(m_tp.maxOutsider()));
}

void MainWindow::updateTotePrediction()
{
    m_tp.setMinHome(ui->minhEdit->text().toInt());
    m_tp.setMinDraw(ui->mindEdit->text().toInt());
    m_tp.setMinGuest(ui->mingEdit->text().toInt());
    m_tp.setMinFavorite(ui->minfEdit->text().toInt());
    m_tp.setMinNeutral(ui->minnEdit->text().toInt());
    m_tp.setMinOutsider(ui->minoEdit->text().toInt());
    m_tp.setMaxHome(ui->maxhEdit->text().toInt());
    m_tp.setMaxDraw(ui->maxdEdit->text().toInt());
    m_tp.setMaxGuest(ui->maxgEdit->text().toInt());
    m_tp.setMaxFavorite(ui->maxfEdit->text().toInt());
    m_tp.setMaxNeutral(ui->maxnEdit->text().toInt());
    m_tp.setMaxOutsider(ui->maxoEdit->text().toInt());
}

void MainWindow::onSavePrediction()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Выберите файл", QString(getenv("HOME")), tr("Toto Master Prediction Files (*.tmpf)"));

    if (fileName.isNull())
        return;

    if (!QFile::exists(fileName))
        fileName.append(".tmpf");

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Ошибка", "Неудалось открыть файл");
        return;
    }
    QDataStream stream(&file);
    stream << m_tp;
    file.close();
}

void MainWindow::onLoadPrediction()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Выберите файл", QString(getenv("HOME")), tr("Toto Master Prediction Files (*.tmpf)"));

    if (fileName.isNull())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Ошибка", "Неудалось открыть файл");
        return;
    }
    QDataStream stream(&file);
    stream >> m_tp;
    file.close();

    this->updateTotePredictionView();
    this->calcAccuracy();
}

void MainWindow::onLoadPacket()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Выберите файл", QString(getenv("HOME")), tr("Text Files (*.txt)"));

    if (fileName.isNull())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Ошибка", "Неудалось открыть файл");
        return;
    }

    m_packet.clear();
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        Coupon c;
        int i;
        if (15 == m_ec)
            i = line.indexOf("(");      // fon
        else
            i = line.indexOf("=");      //city
        while( i > -1) {
            i++;
            c.append(QString(line.at(i)).toInt());
            if (15 == m_ec)
                i = line.indexOf("(", i);       //fon
            else
                i = line.indexOf("=", i);      //city
        }
        m_packet.append(c);
    }

    file.close();
    ui->countEdit->setText(QString::number(m_packet.count()));

    this->calcAccuracy();
    this->calculateResult();
}

void MainWindow::calculateResult()
{
    int winCouponsCount = 0;
    m_packetresult.clear();
    for(int i = 0; i < m_ec + 1; i++)
        m_packetresult.append(0);
    for(int i = 0; i < m_packet.count(); i++) {
        int mc = this->matchsCount(m_resultmodel.result(), m_packet.at(i));
        if (-1 < mc) {
            for (int j = mc; 0 < j; j--)
                m_packetresult[j]++;
            if (mc > 8)
                winCouponsCount++;
        }
    }

    m_calcmodel.setResult(m_packetresult);
}

void MainWindow::onWonSum(const qint64 &sum)
{
    ui->totalWonEdit->setText(QString::number(sum));
    int copcost = 30;
    if (15 == m_ec)
        copcost = 50;
    int tspent = copcost * m_packet.count();
    int tprofit = sum - tspent;
    ui->totalSpetEdit->setText( QString::number(tspent) );
    ui->totalProfitEdit->setText( QString::number(tprofit) );
}

void MainWindow::onImport()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Выберите файл", QString(getenv("HOME")), tr("Text Files (*.txt)"));

    if (fileName.isNull())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Ошибка", "Неудалось открыть файл");
        return;
    }

    m_import.clear();
    QTextStream in(&file);
    int lc = 0;
    while (!in.atEnd()) {
        lc++;
        QString line = in.readLine().split("|").last();
        QStringList valList = line.split(";");
        if ( valList.first().contains( QRegExp("\\d+..\\d+") ) ) {
            valList.removeFirst();
            for (int i = 0; i < valList.count(); i++)
                valList[i].remove( QRegExp("\\d+=") );
        }

        if (15 != valList.count())
            continue;
        Packet temp;
        Coupon c;
        for(int i = 0; i < valList.count() - 1; i++)
            c.append(-1);
        temp.append(c);

        for(int i = 0; i < valList.count() - 1; i++) {
            QStringList vals = valList.at(i).split(",");

            if (1 == vals.count())
                for(int j = 0; j < temp.count(); j++)
                    temp[j][i] = vals.at(0).toInt();

            if (2 == vals.count()) {
                Packet temp2 = temp;
                for(int j = 0; j < temp.count(); j++)
                    temp[j][i] = vals.at(0).toInt();

                for(int j = 0; j < temp2.count(); j++)
                    temp2[j][i] = vals.at(1).toInt();
                temp.append(temp2);
            }

            if (3 == vals.count()) {
                Packet temp2 = temp;
                Packet temp3 = temp;
                for(int j = 0; j < temp.count(); j++)
                    temp[j][i] = vals.at(0).toInt();

                for(int j = 0; j < temp2.count(); j++)
                    temp2[j][i] = vals.at(1).toInt();
                temp.append(temp2);

                for(int j = 0; j < temp3.count(); j++)
                    temp3[j][i] = vals.at(2).toInt();
                temp.append(temp3);
            }
        }

        for(int i = 0; i < temp.count(); i++)
            if ( !m_import.contains(temp.at(i)) )
                m_import.append(temp.at(i));
    }

    file.close();
    ui->importCountEdit->setText(QString::number(m_import.count()));
}

int MainWindow::getRandomResult(int t1p, int txp, int t2p)
{
    int val = -1;
    if (100 != t1p + txp + t2p)
        return val;
    int z = qrand() % 100 + 1;
    if (0 < z && z <= t1p )
        val = 1;
    else if (t1p < z && z <= t1p + txp )
        val = 0;
    else if (txp < z && z <= t1p + txp + t2p )
        val = 2;

    return val;
}

void MainWindow::maxOffset(const TotePrediction &tp, const Packet &pack, int &event, int &result)
{
      event = -1;
      result = -1;
      double maxOff = 0.0;
      for(int i = 0; i < tp.eventsCount(); i++) {
          int h = 0;
          int d = 0;
          int g = 0;
          for (int j = 0; j < pack.count(); j++) {
              if (1 == pack.at(j).at(i))
                  h++;
              if (0 == pack.at(j).at(i))
                  d++;
              if (2 == pack.at(j).at(i))
                  g++;
          }
          double hof =  (double) tp.at(i).home() - ((double) h / (double) pack.count() * 100) ;
          double dof =  (double) tp.at(i).draw() - ((double) d / (double) pack.count() * 100) ;
          double gof =  (double) tp.at(i).guest() - ((double) g / (double) pack.count() * 100) ;
          if (hof > maxOff) {
              maxOff = hof;
              event = i;
              result = 1;
          }
          if (dof > maxOff) {
              maxOff = dof;
              event = i;
              result = 0;
          }
          if (gof > maxOff) {
              maxOff = gof;
              event = i;
              result = 2;
          }
      }
}

void MainWindow::calcAccuracy()
{
    QList<QList<double> > ctp;
    for(int i = 0; i < m_tp.eventsCount(); i++) {
        int h = 0;
        int d = 0;
        int g = 0;
        QList<double> cep;
        for (int j = 0; j < m_packet.count(); j++) {
            if (1 == m_packet.at(j).at(i))
                h++;
            if (0 == m_packet.at(j).at(i))
                d++;
            if (2 == m_packet.at(j).at(i))
                g++;
        }
        cep.append((double) h / (double) m_packet.count() * 100);
        cep.append((double) d / (double) m_packet.count() * 100);
        cep.append((double) g / (double) m_packet.count() * 100);
        ctp.append(cep);
//        qDebug() << QString("%1: %2, %3, %4").arg(QString::number(i+1),
//                                                  QString::number((double) h / (double) m_packet.count() * 100),
//                                                  QString::number((double) d / (double) m_packet.count() * 100),
//                                                  QString::number((double) g / (double) m_packet.count() * 100));
    }
    m_model.setAccuracy(ctp);
}

int MainWindow::valueCount(int value, const Coupon &c) const
{
    int res = 0;
    for(int i = 0; i < c.count(); i++)
        if (value == c.at(i))
            res++;
    return res;
}

int MainWindow::allocationCount(int lev, const Coupon &c, const TotePrediction &tp)
{
    int res = 0;
    for (int i = 0; i < c.count(); i++) {
        int value = tp.at(i).neutral();
        if (0 < lev)
            value = tp.at(i).favorite();
        if (0 > lev)
            value = tp.at(i).outsider();
        if (value == c.at(i))
            res++;
    }

    return res;
}

int MainWindow::matchsCount(const Coupon &c1, const Coupon &c2) const
{
    if (m_ec != c1.count() || m_ec != c2.count())
        return -1;
    int res = 0;
    for (int i = 0; i < c1.count(); i++)
        if (c1.at(i) == c2.at(i) || -1 == c1.at(i))
            res++;
    return res;
}

