#ifndef GRAPHSDIGITISER_H
#define GRAPHSDIGITISER_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QString>
#include <QFileDialog>
#include <QPixmap>
#include <QVector>
#include <QMouseEvent>
#include <QInputDialog>
#include <QtMath>
#include <QDebug>

namespace Ui {
class graphsdigitiser;
}

class graphsdigitiser : public QMainWindow
{
    Q_OBJECT

public:
    explicit graphsdigitiser(QWidget *parent = nullptr);
    ~graphsdigitiser();

    void mousePressEvent(QMouseEvent *event);
    void calibrationXCheck(); // Function to check if the calibration phase of x-axis is finished or not
    void calibrationYCheck(); // Function to check if the calibration phase of y-axis is finished or not
    double CalculateDistance(QVector<QPointF>& clickedPoints);
    QVector<QPointF> CalculateResult(QVector<QPointF> &chosenPoints, int typeOfX, int typeOfY);

private slots:
    void on_actionOpen_from_a_graph_triggered();

    void on_actionCalibrate_triggered();

    void on_actionManual_Mode_triggered();

    void on_actionLinear_Linear_triggered();

    void on_actionLinear_Log_triggered();

    void on_actionLog_Linear_triggered();

    void on_actionLog_Log_triggered();

private:
    Ui::graphsdigitiser *ui;
    QGraphicsScene *scene = new QGraphicsScene();
    
    QVector<QPointF> CalibrationVectorX; // Vector for saving the calibration points of X-axis
    QVector<QPointF> CalibrationVectorY; // Vector for saving the calibration points of Y-axis
    bool isCalibratingX = false; // flag to indicate whether the app is calibrating X-axis or not
    bool isCalibratingY = false; // flag to indicate whether the app is calibrating Y-axis or not
    bool isCalibrationFinished = false; // flag to indicate whether the calibration is finished and the app is ready to do digitisation
    bool isCalibrationLog = false; // flag to indicate whether the app is calibrating log axis or not
    bool isCalibrationLinear = false; // flag to indicate whether the app is calibrating linear axis or not

    QVector<QPointF> PointsVector; // Vector for saving the points need to be digitised
    QVector<QPointF> DigitisedPointsVector; //Vector for saving the points that have been digitised
    bool isDigitising = false; // flag to indicate whether the app is in digitising mode or not
    
    double XAxis_min = 0; // Varaible to save the physical value of the minimum of the X axis
    double XAxis_max = 0; // Varaible to save the physical value of the maximum of the X axis
    double YAxis_min = 0; // Varaible to save the physical value of the minimum of the Y axis
    double YAxis_max = 0; // Varaible to save the physical value of the maximum of the Y axis
    double Calibration_factorX = 0; // Varaible to save the calibration factor of X-axis (The unit depends on the graph)
    double Calibration_factorY = 0; // Varaible to save the calibration factor of Y-axis (The unit depends on the graph)

    int type_of_x = 0;
    int type_of_y = 0;
};

#endif // GRAPHSDIGITISER_H
