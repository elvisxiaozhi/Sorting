#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFile>
#include <algorithm> // std::random_shuffle
#include <ctime>
#include <cstdlib>
#include <QDebug>
#include <thread>
#include <chrono>
#include <QtConcurrent/QtConcurrent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    std::srand(unsigned(std::time(0)));
    createLabels();
    sleepTime = ui->sortingSpeedBar->value();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::createLabels()
{
    ui->gridLayout->setSpacing(1);
    int i, j;
    for (i = 0; i < 100; ++i) {
        QLabel *lbl = new QLabel();
        QString path = QString(":/icons/%1.gif").arg(QString::number(i + 1));
        lbl->setPixmap(QPixmap(path).scaled(50, 50, Qt::KeepAspectRatio));
        lbl->setFixedSize(50, 50);
        lbl->setObjectName(QString::number(i));

        lblVec.push_back(lbl);
    }

    std::random_shuffle(lblVec.begin(), lblVec.end());

    for (i = 0; i < 10; ++i) {
        for (j = 0; j < 10; ++j) {
            ui->gridLayout->addWidget(lblVec[(i * 10) + j], i, j);
        }
    }
}

void Widget::quickSort(int low, int high)
{
    if (low < high) {
        int pivot = quickSortPartition(low, high);

        quickSort(low, pivot - 1);
        quickSort(pivot + 1, high);
    }
}

void Widget::swapWidget(int from, int to)
{
    mutex.lock();

    int fromX = from / 10;
    int fromY = from % 10;
    int toX = to / 10;
    int toY = to % 10;

    QWidget *fromWidget = ui->gridLayout->itemAtPosition(fromX, fromY)->widget();
    QWidget *toWidget = ui->gridLayout->itemAtPosition(toX, toY)->widget();
    ui->gridLayout->removeWidget(fromWidget);
    ui->gridLayout->removeWidget(toWidget);
    ui->gridLayout->addWidget(fromWidget, toX, toY);
    ui->gridLayout->addWidget(toWidget, fromX, fromY);

    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    mutex.unlock();
}

int Widget::quickSortPartition(int low, int high)
{
    int pivot = lblVec[high]->objectName().toInt();
    int i = low - 1, j;
    for (j = low; j <= high - 1; ++j) {
        if (lblVec[j]->objectName().toInt() < pivot) {
            ++i;

            std::swap(lblVec[i], lblVec[j]);
            QtConcurrent::run(this, &Widget::swapWidget, i, j);
        }
    }

    std::swap(lblVec[i + 1], lblVec[high]);
    QtConcurrent::run(this, &Widget::swapWidget, i + 1, high);

    return (i + 1);
}

void Widget::on_startSortingBtn_clicked()
{
    quickSort(0, lblVec.size() - 1);
}

void Widget::on_shuffleBtn_clicked()
{
    std::random_shuffle(lblVec.begin(), lblVec.end());

    int i, j;
    for (i = 0; i < 10; ++i) {
        for (j = 0; j < 10; ++j) {
            ui->gridLayout->removeWidget(ui->gridLayout->itemAtPosition(i, j)->widget());
        }
    }

    for (i = 0; i < 10; ++i) {
        for (j = 0; j < 10; ++j) {
            ui->gridLayout->addWidget(lblVec[(i * 10) + j], i, j);
        }
    }
}

void Widget::on_sortingSpeedBar_valueChanged(int value)
{
    sleepTime = value;
}
