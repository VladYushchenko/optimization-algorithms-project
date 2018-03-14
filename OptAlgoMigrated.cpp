#include "OptAlgoMigrated.h"
#include "ui_OptAlgoMigrated.h"
#include <sstream>

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


    connect(ui->generateButton, SIGNAL(clicked()), this, SLOT(generateData()));
    connect(ui->solveButton, SIGNAL(clicked()), this, SLOT(solveAndDraw()));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::generateData()
{
    unsigned int rectCount = ui->rectangleNumberSpinBox->value();
    unsigned int minRectLength = ui->leftBoundSpinBox->value();
    unsigned int maxRectLength = ui->rightBoundSpinBox->value();
    unsigned int boxLength = ui->boxSizeSpinBox->value();

	
	auto generatorTypeIndex = ui->generatorBox->currentIndex();
	auto message = ui->generatorBox->itemText(generatorTypeIndex);
	ui->logger->insertPlainText("Generating Instances with " + message + "\n");

	std::ostringstream stream;
	stream << "Parameters:\n Rectangles - " << rectCount << std::endl
		   << " Sides - [" << minRectLength << "," << maxRectLength << "]" << std::endl
		   << " Box Length - " << boxLength << std::endl;
	ui->logger->insertPlainText(QString::fromStdString(stream.str()));
	ui->logger->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);

    problem = new OptimizationProblem(rectCount, boxLength, minRectLength, maxRectLength, generatorTypeIndex);

    ui->logger->insertPlainText("Instance Generated\n\n");
	ui->logger->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
	
	auto solutionAnswer = std::to_string(problem->getStartSolution().size());
	message = QString::fromStdString("Initial solution contains " + solutionAnswer + " boxes.\n\n");
	ui->logger->insertPlainText(message);
	ui->logger->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
}

void delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);
}


void Widget::solveAndDraw()
{
	ui->logger->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
 
	auto solutionIndex = ui->methodComboBox->currentIndex();
	auto message = ui->methodComboBox->itemText(solutionIndex);
	ui->logger->insertPlainText("Solving with " + message + " method\n\n");
	ui->logger->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);

    problem->solveOptimizationProblem(ui->methodComboBox->currentIndex());
    solutionInfo = problem->getSolutionHistory();

	auto solutionMessage = "Program terminated in " + std::to_string(solutionInfo.size() - 1) + " steps.\n";
	solutionMessage += "Solution: " + std::to_string(solutionInfo.back().size()) + " boxes\n\n";
	ui->logger->insertPlainText(QString::fromStdString(solutionMessage));
	ui->logger->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);

    outlinePen->setWidth(1);

    const int OFFSET = 5;
    const int numberBoxesInRow = static_cast<int>(std::sqrt(solutionInfo.front().size()));

    while (!solutionInfo.empty() && this->isVisible()){

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

