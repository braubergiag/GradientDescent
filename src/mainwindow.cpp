#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    counterPlot_ = new QCustomPlot();
    ui->verticalLayout->addWidget(counterPlot_);
    model_ = new Model();

}

MainWindow::~MainWindow()
{
    delete counterPlot_;
    delete model_;
    delete ui;
}

void MainWindow::loadCounterPlot( QCustomPlot * customPlot)
{
    if (customPlot){
        ui->verticalLayout->addWidget(customPlot);
    }
}



QCustomPlot *MainWindow::createCounterPlot()
{
    QCustomPlot * customPlot = new QCustomPlot();
    //customPlot->setInteractions(QCP::iRangeDrag);
    customPlot->axisRect()->setupFullAxesBox(true);
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");

    // set up the QCPColorMap:
    QCPColorMap *colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
    int nx = 200; //model_->gd().historyByCoord().at(0).size();
    int ny = 200; //model_->gd().historyByCoord().at(0).size();
    colorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points

    std::vector<Domain> domain = model_->functionHandler().getFunctionDomain();
    double x1Min,x1Max,x2Min,x2Max;
    x1Min = domain.at(0).first;
    x1Max = domain.at(0).second;

    x2Min = domain.at(1).first;
    x2Max=  domain.at(1).second;



    colorMap->data()->setRange(QCPRange(x1Min, x1Max), QCPRange(x2Min, x2Max)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
    // now we assign some data, by accessing the QCPColorMapData instance of the color map:
    double x, y, z;
    for (int xIndex=0; xIndex<nx; ++xIndex )
    {
      for (int yIndex=0; yIndex<ny; ++yIndex)
      {
        colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
        auto f = model_->functionHandler().getObjectFunction();
        z = f({x,y});
        colorMap->data()->setCell(xIndex, yIndex, z);
      }
    }

    // add a color scale:
    QCPColorScale *colorScale = new QCPColorScale(customPlot);
    customPlot->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
    colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    colorMap->setColorScale(colorScale); // associate the color map with the color scale
    colorScale->axis()->setLabel("Function Range");

    // set the color gradient of the color map to one of the presets:
    colorMap->setGradient(QCPColorGradient::gpThermal);
    // we could have also created a QCPColorGradient instance and added own colors to
    // the gradient, see the documentation of QCPColorGradient for what's possible.

    // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
    colorMap->rescaleDataRange();

    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup *marginGroup = new QCPMarginGroup(customPlot);
    customPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    // rescale the key (x) and value (y) axes so the whole color map is visible:
    customPlot->rescaleAxes();




    //QCustomPlot * customPlot = new QCustomPlot();

    QVector<double> x1 = QVector<double>(model_->gd().historyByCoord().at(0).begin(),
                                        model_->gd().historyByCoord().at(0).end());
    QVector<double> y1 = QVector<double>(model_->gd().historyByCoord().at(1).begin(),
                                        model_->gd().historyByCoord().at(1).end());


    customPlot->addGraph();
    customPlot->graph(0)->setData(x1,y1);
    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDot));
    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(180,180,180));
    customPlot->graph(0)->setPen(pen);


    QString graphTitle = QString("Search curve");
    customPlot->graph(0)->setName(graphTitle);

    customPlot->xAxis->setLabel("x1");
    customPlot->yAxis->setLabel("x2");
    customPlot->xAxis->setRange(x1Min,x1Max);
    customPlot->yAxis->setRange(x2Min,x2Max);


    QVector<double> ticks;
    QVector<QString> labels;
//    for (const auto & value: model->sampleSizeInterval()){
//        ticks << value;
//        labels << QString::number(value);
//    }


//    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
//    textTicker->addTicks(ticks, labels);
//    customPlot->xAxis->setTicker(textTicker);
//    customPlot->xAxis->setTickLabelRotation(60);

    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    customPlot->legend->setBrush(QColor(255, 255, 255, 100));
    customPlot->legend->setBorderPen(Qt::NoPen);
//     QFont legendFont = font();
//     legendFont.setPointSize(10);
//     customPlot->legend->setFont(legendFont);
    //customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);



    QString info = QString::fromStdString(model_->generateLog());
    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, info, QFont("sans", 10, QFont::Normal)));
    customPlot->replot();

    return customPlot;
}


void MainWindow::on_actionSet_Model_triggered()
{
    DialogModel * dlg = new DialogModel(this,model_);
    dlg->exec();
    if (dlg->result() == QDialog::Accepted){
        model_->run();
        if (counterPlot_) {
            ui->verticalLayout->removeWidget(counterPlot_);
            delete counterPlot_;
        }
        if (model_->functionHandler().getDim() == 2){
            counterPlot_ = new QCustomPlot();
            counterPlot_ = createCounterPlot();
            loadCounterPlot(counterPlot_);
        }

    }

}

