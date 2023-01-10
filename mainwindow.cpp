
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QStatusBar>
#include <QUrl>
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
#ifndef _WIN32
    setlocale(LC_NUMERIC,"C");         // C-Standard needed for PI ., Problem in JSON printf and german PI.
#endif
    m_Timer=nullptr;
    m_Settings = new QSettings("klosterallee.de","DPS3005");
    serverEdit = 1;
    m_ConnectedState = 0;
    ui->setupUi(this);
    initRs232Modbus();  // auto connect on start
    initActions();
}

MainWindow::~MainWindow()
{
    if( modbusDevice )
    {
        modbus_close( modbusDevice );
        modbus_free( modbusDevice );
        modbusDevice = NULL;
    }
    delete ui;
}

void MainWindow::initActions()
{
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionExit->setEnabled(true);
    ui->actionOptions->setEnabled(true);

    connect(ui->connectButton, &QPushButton::clicked,this, &MainWindow::onConnectButtonClicked);
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::onConnectButtonClicked);
    connect(ui->actionDisconnect, &QAction::triggered,this, &MainWindow::onConnectButtonClicked);

    //connect(ui->readDPS, &QPushButton::clicked, this, &MainWindow::onReadDPS);
    connect(ui->pb1V, &QPushButton::clicked, this, &MainWindow::onpb1V);
    connect(ui->pb8V, &QPushButton::clicked, this, &MainWindow::onpb8V);
    connect(ui->pb12V, &QPushButton::clicked, this, &MainWindow::onpb12V);
    connect(ui->pb138V, &QPushButton::clicked, this, &MainWindow::onpb13_8V);

    connect(ui->actionExit, &QAction::triggered, this, &QMainWindow::close);

    QString port = m_Settings->value("PORT").toString();
    if (!port.isEmpty())
    {
        //qDebug() << "TEST:" << port;
        ui->portEdit->setText(port);
        ui->l_VI->setText("VI:");

        // connect...
        // start timer.
        onConnectButtonClicked();
    }
}

void MainWindow::pollForDataOnBus( void )
{
    if( modbusDevice )
    {
        modbus_poll( modbusDevice );
    }
}

bool MainWindow::initRs232Modbus()
{
    qDebug() << "initRs232Modbus";

    if (modbusDevice) {
        modbus_close( modbusDevice );
        modbus_free( modbusDevice );
        modbusDevice = nullptr;
    }

    modbusAdr = 1; // we use address 1
    QString port = m_Settings->value("PORT").toString();
    modbusDevice = modbus_new_rtu( port.toLatin1().constData(), 9600, 'N', 8, 1 );

    if( modbus_connect( modbusDevice ) == -1 )
    {
        qDebug() << "Could not connect serial port!" << port ;
        //emit connectionError( tr( "Could not connect serial port!" ) );

        modbus_close( modbusDevice );
        modbus_free( modbusDevice );
        modbusDevice = nullptr;
        return false;
    }
    qDebug() << "initRs232Modbus ok " << port;

    QTimer * t = new QTimer( this );
    connect( t, SIGNAL(timeout()), this, SLOT(pollForDataOnBus()));
    t->start( 5 );

    return true;
}

void MainWindow::onConnectButtonClicked()
{
    qDebug() << "onConnectButtonClicked";

    m_ConnectedState = 0;
    statusBar()->clearMessage();

    QString ap = ui->portEdit->text();
    if (!ap.isEmpty())
    {
        m_Settings->setValue("PORT", ap);
        qDebug() << "write PORT for auto open on next start: " << ap;
    }

    bool connected = initRs232Modbus();  // connect on click
    if (connected)
    {
        m_Timer = new QTimer(this);
        connect(m_Timer, &QTimer::timeout, this, &MainWindow::onSecond);
        m_Timer->start(1000);

    }
}

void MainWindow::onpb8V()
{
    onSetUint(0,800);
}

void MainWindow::onpb12V()
{
    onSetUint(0,1200);
}

void MainWindow::onpb13_8V()
{
    onSetUint(0,1380);
}

void MainWindow::onpb1V()
{
    onSetUint(0,100);  // 0 -> VOUT
}

void MainWindow::onSetUint(int no, int data)
{
    qDebug() << "onSetUint " << no << " data: " << data;  // we can only set 1V abobe VIN!!! TODO there is no messagte
    if (!modbusDevice)
        return;

   modbus_set_slave( modbusDevice, 1 );

   int  ret = modbus_write_register( modbusDevice, no,data);
   qDebug() << "modbus_write_register: " << ret;
}

void MainWindow::onReadDPS()
{
    if (!modbusDevice)
        return;

    statusBar()->clearMessage();

    modbus_set_slave( modbusDevice, 1 );

    int ret = modbus_read_registers( modbusDevice, modbusAdr, 12, modbusData );

    //qDebug() << "modbus_read_registers: " << ret;

    if (ret == 12)
    {
        for (int i = 0; i < ret; i++)
        {
            int data = modbusData[i];
            //qDebug() << i << "modbus_read_registers 0: " << data;
            dpsData.setValue(i+1,data);
        }
        updateData();
    }
}

void MainWindow::onSecond()
{
   if (modbusDevice)
   {
       if (m_ConnectedState==0)
       {
           m_ConnectedState = 1;
           onSetUint(1,1000);  // 0 -> IMAX limit to 1A
           onSetUint(9,1);  // ON
           statusBar()->showMessage("1A and ON");
       }

       onReadDPS();
       return;
   }
   qDebug() << "MainWindow::OnSecond but closed -> stopping";

   //m_Timer->stop();

}

QString MainWindow::getValuesAsJson()
{    
    QString ret="{";

    QString  iset = dpsData.getValue(1);
    QString  vout = dpsData.getValue(2);
    QString  iout = dpsData.getValue(3);
    QString  pow  = dpsData.getValue(4);
    QString  vin  = dpsData.getValue(5);

    ret+= "\"vout\": \"" + vout + "\", ";
    ret+= "\"vin\":  \"" + vin  + "\", ";
    ret+= "\"iout\": \"" + iout + "\", ";
    ret+= "\"iset\": \"" + iset + "\", ";
    ret+= "\"pow\":  \"" + pow  + "\", ";
    ret+= "\"ver\":  \"1.1\"";   // VERSION-TAG  float !!! only first 2 levels!
    ret += "}\n\r";

    return ret;
}

float MainWindow::getActVoltage()
{
    float vo = dpsData.getVO();
    return vo;
}

void MainWindow::updateData()
{
    dpsData.dumpData();

    QString VI = dpsData.getValue(5);
    QString VO = dpsData.getValue(2);

    QString IO = dpsData.getValue(3);

    ui->lcdVI->setDigitCount(4);

    ui->lcdVI->display(VI );
    ui->lcdVI->setStyleSheet("background: black; color: #008080");

    ui->lcdV->setDigitCount(4);
    ui->lcdV->display(VO);
    ui->lcdV->setStyleSheet("background: black; color: green");

    ui->lcdIO->setDigitCount(4);
    ui->lcdIO->display(IO);
    ui->lcdIO->setStyleSheet("background: black; color: yellow");
}
