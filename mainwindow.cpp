#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   m_circleWidget = new CircleAntennaWidget(this);
   m_verticalWidget = new VerticalAntennaWidget(this);

   QHBoxLayout* mainLayout = new QHBoxLayout;
   mainLayout->addWidget(m_circleWidget);
   mainLayout->addWidget(m_verticalWidget);

   auto centralWidget = new QWidget;
   centralWidget->setLayout(mainLayout);
   setCentralWidget(centralWidget);
}
MainWindow::~MainWindow()
{
    delete ui;
}

