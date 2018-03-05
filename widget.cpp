#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    redBrush = new QBrush(Qt::red);
    greenBrush = new QBrush (Qt::green);
    outlinePen = new QPen (Qt::black);


    connect(ui->pushButton , SIGNAL(clicked()), this, SLOT(generateData()));
    connect(ui->pushButton_2 , SIGNAL(clicked()), this, SLOT(solveAndDraw()));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::generateData()
{
    unsigned int rectCount = ui->spinBox->value();
    unsigned int minRectLength = ui->spinBox_2->value();
    unsigned int maxRectLength = ui->spinBox_3->value();
    unsigned int boxLength = ui->spinBox_4->value();

    problem = new OptimizationProblem(rectCount, boxLength, minRectLength, maxRectLength);
    std::cout << "Generating..." << std::endl;
    std::cout << problem->getStartSolution().size() << std::endl;
}

void delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);
}


void Widget::solveAndDraw()
{
    std::cout << "Solving..." << std::endl;
    std::cout << ui->comboBox->currentIndex() << std::endl;
    problem->solveOptimizationProblem(ui->comboBox->currentIndex());
    solutionInfo = problem->getSolutionHistory();
    std::cout << "Program terminated in " << solutionInfo.size() - 1 << " steps." << std::endl;
    std::cout << "Solution is " << solutionInfo.back().size() << " boxes" << std::endl;

    std::cout << "Drawing..." << std::endl;

    outlinePen->setWidth(1);

    const int OFFSET = 5;
    const int numberBoxesInRow = static_cast<int>(std::sqrt(solutionInfo.front().size()));

    while (!(solutionInfo.empty())){

        scene->clear();
        ui->graphicsView->viewport()->update();

        auto it = solutionInfo.begin();

        for (size_t i = 0; i < (*it).size(); i++)
        {
            // Box output
            int boxOffsetX = ((*it)[i]._boxWidth + OFFSET) * (i % numberBoxesInRow);

            int boxOffsetY = ((*it)[i]._boxHeigth + OFFSET) * (i / numberBoxesInRow);
            scene->addRect(boxOffsetX, boxOffsetY,
                           (*it)[i]._boxWidth, (*it)[i]._boxHeigth,
                           *outlinePen, *redBrush);

            std::vector<Rectangle> stlRectagles = (*it)[i]._containedRectangles;
            for (size_t j = 0; j < stlRectagles.size(); j++)
                scene->addRect(stlRectagles[j].x + boxOffsetX, stlRectagles[j].y + boxOffsetY,
                               stlRectagles[j].width, stlRectagles[j].height,
                               *outlinePen, *greenBrush);
        }
        solutionInfo.pop_front();
        delay();

    }

}

