#include "graphsdigitiser.h"
#include "ui_graphsdigitiser.h"

graphsdigitiser::graphsdigitiser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::graphsdigitiser)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(scene);

    setCentralWidget(ui->graphicsView);
}

graphsdigitiser::~graphsdigitiser()
{
    delete ui;

    delete scene;
}

void graphsdigitiser::mousePressEvent(QMouseEvent *event)
{
    if(isCalibratingY)
    {
        CalibrationVectorY.push_back(event->pos());
        calibrationYCheck();
    }
    if(isCalibratingX)
    {
        CalibrationVectorX.push_back(event->pos());
        calibrationXCheck();
    }
    if(isDigitising)
    {
        PointsVector.push_back(event->pos());
        QVector<QPointF> a;
        a = CalculateResult(PointsVector, type_of_x, type_of_y);
        qDebug()<<"x: "<<a[a.size()-1].x()<<"y: "<<a[a.size()-1].y();
    }
}

void graphsdigitiser::calibrationXCheck() // This function is called to check and finish calibration of x-axis
{
    if(CalibrationVectorX.size() == 2)
    {
        isCalibratingX = false;
        bool ok;
        XAxis_min = QInputDialog::getDouble(this, tr("Calibration Factor of x-axis"), tr("Please enter the minimum of the x-axis:"), 0, -2147483648, 2147483657, 1, &ok);
        XAxis_max = QInputDialog::getDouble(this, tr("Calibration Factor of x-axis"), tr("Please enter the maximum of the x-axis:"), 0, -2147483648, 2147483657, 1, &ok);
        Calibration_factorX = CalculateDistance(CalibrationVectorX)/(XAxis_max - XAxis_min);
        isCalibratingY = true;
        CalibrationVectorY.clear();
    }
}

void graphsdigitiser::calibrationYCheck() // This function is called to check and finish calibration of y-axis
{
    if(CalibrationVectorY.size() == 2)
    {
        isCalibratingY = false;
        bool ok;
        YAxis_min = QInputDialog::getDouble(this, tr("Calibration Factor of y-axis"), tr("Please enter the minimum of the y-axis:"), 0, -2147483648, 2147483657, 1, &ok);
        YAxis_max = QInputDialog::getDouble(this, tr("Calibration Factor of y-axis"), tr("Please enter the maximum of the y-axis:"), 0, -2147483648, 2147483657, 1, &ok);
        Calibration_factorY = CalculateDistance(CalibrationVectorY)/(YAxis_max - YAxis_min);
    }
}

double graphsdigitiser::CalculateDistance(QVector<QPointF> &clickedPoints)
{
    double pixel_distance = 0;
    int i = 1;
    pixel_distance = qSqrt(qPow(clickedPoints[i].x() - clickedPoints[i - 1].x(), 2) + qPow(clickedPoints[i].y() - clickedPoints[i - 1].y(), 2));
    return pixel_distance;
}

QVector<QPointF> graphsdigitiser::CalculateResult(QVector<QPointF> &chosenPoints, int typeOfX, int typeOfY)
{
    QVector<QPointF> DigitisedPointsVector; //Vector for saving the points that have been digitised
    if(typeOfX == 1 && typeOfY == 1)
    {
        for(int i = 0;i < chosenPoints.size();i++)
        {
            DigitisedPointsVector.push_back(QPointF((PointsVector[i].x()-CalibrationVectorX[0].x())/Calibration_factorX + XAxis_min,(CalibrationVectorY[0].y()-PointsVector[i].y())/Calibration_factorY + YAxis_min));
        }
        return DigitisedPointsVector;
    }
    if(typeOfX == 1 && typeOfY == 2)
    {
        for(int i = 0;i < chosenPoints.size();i++)
        {
            DigitisedPointsVector.push_back(QPointF((PointsVector[i].x()-CalibrationVectorX[0].x())/Calibration_factorX + XAxis_min,qPow(10,(CalibrationVectorY[0].y()-PointsVector[i].y())/Calibration_factorY + YAxis_min)));
        }
        return DigitisedPointsVector;
    }
    if(typeOfX == 2 && typeOfY == 1)
    {
        for(int i = 0;i < chosenPoints.size();i++)
        {
            DigitisedPointsVector.push_back(QPointF(qPow(10,(PointsVector[i].x()-CalibrationVectorX[0].x())/Calibration_factorX + XAxis_min),(CalibrationVectorY[0].y()-PointsVector[i].y())/Calibration_factorY + YAxis_min));
        }
        return DigitisedPointsVector;
    }
    if(typeOfX == 2 && typeOfY == 2)
    {
        for(int i = 0;i < chosenPoints.size();i++)
        {
            DigitisedPointsVector.push_back(QPointF(qPow(10,(PointsVector[i].x()-CalibrationVectorX[0].x())/Calibration_factorX + XAxis_min),qPow(10,(CalibrationVectorY[0].y()-PointsVector[i].y())/Calibration_factorY + YAxis_min)));
        }
        return DigitisedPointsVector;
    }
}

void graphsdigitiser::on_actionOpen_from_a_graph_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Load graph")); // Allows the user to choose an input graph

    QPixmap map(filename);

    scene->addPixmap(map);
}


void graphsdigitiser::on_actionCalibrate_triggered() // Called whenever the user clicks on "Calibrate"
{
    isCalibratingX = true; // Change the status of the flag to active the calibrate mode
    CalibrationVectorX.clear();
    CalibrationVectorY.clear(); // Clear two calibration vectors for re-calibrating
}

void graphsdigitiser::on_actionManual_Mode_triggered() // Called whenever the user clicks on "Manual Mode"
{
    isDigitising = true;
    PointsVector.clear();
}

void graphsdigitiser::on_actionLinear_Linear_triggered() // Called whenever the user clicks on "Linear-Linear"
{
    type_of_x = 1;
    type_of_y = 1;
}

void graphsdigitiser::on_actionLinear_Log_triggered() // Called whenever the user clicks on "Linear-Log"
{
    type_of_x = 1;
    type_of_y = 2;
}

void graphsdigitiser::on_actionLog_Linear_triggered() // Called whenever the user clicks on "Log-Linear"
{
    type_of_x = 2;
    type_of_y = 1;
}

void graphsdigitiser::on_actionLog_Log_triggered() // Called whenever the user clicks on "Log-Log"
{
    type_of_x = 2;
    type_of_x = 2;
}
