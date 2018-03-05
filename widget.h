#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QRect>
#include <QTime>
#include <QPushButton>
#include "optimizationproblem.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void generateData();
    void solveAndDraw();

private:
    Ui::Widget *ui;

    OptimizationProblem *problem;
    QGraphicsScene *scene;
    QBrush *greenBrush;
    QBrush *redBrush;
    QPen *outlinePen;

    std::list<std::vector<Box>> solutionInfo;
};

#endif // WIDGET_H
