
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE

#include "modbus.h"

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class WriteRegisterModel;

#include "dpsdata.h"
#include <QSettings>
#include <QTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
   void onSetUint(int no, int data);
   void onSetIint(int no, int data);
  float getActVoltage();
  QString getValuesAsJson();
private:
    void initActions();

    public slots:
    void onSecond();


private slots:
    void onConnectButtonClicked();
    void pollForDataOnBus( void );
    void onReadDPS();
    void onpb13_8V();
    void onpb12V();
    void onpb8V();
    void onpb1V();
    void updateData();
    bool initRs232Modbus();


private:
    Ui::MainWindow *ui = nullptr;
    modbus_t *modbusDevice = nullptr;
    int       modbusAdr;
    uint16_t  modbusData[32];
    int serverEdit;

    DpsData dpsData;
    QSettings *m_Settings;   // PORT is save and if started and port is the is auto connected.
    QTimer *m_Timer;
    int    m_ConnectedState;
};

#endif // MAINWINDOW_H
