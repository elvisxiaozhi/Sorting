#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMutex>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_startSortingBtn_clicked();

    void on_shuffleBtn_clicked();

    void on_sortingSpeedBar_valueChanged(int value);

private:
    Ui::Widget *ui;

    QVector<QLabel *> lblVec;
    QMutex mutex;
    int sleepTime;
    void createLabels();
    void quickSort(int low, int high);
    int quickSortPartition(int low, int high);
    void swapWidget(int from, int to);
};

#endif // WIDGET_H
