/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the QtSerialBus module.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"


#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QUrl>


#include <qdebug.h>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    m_Timer=nullptr;
    m_Settings = new QSettings("klosterallee.de","DPS3005");
    serverEdit = 1;
    ui->setupUi(this);

    m_settingsDialog = new SettingsDialog(this);

    initRs232Modbus();
    initActions();
}

MainWindow::~MainWindow()
{
    if (modbusDevice)
        modbusDevice->disconnectDevice();
    delete modbusDevice;

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

    connect(ui->readDPS, &QPushButton::clicked, this, &MainWindow::onReadDPS);
    connect(ui->pb2V, &QPushButton::clicked, this, &MainWindow::onpb2V);
    connect(ui->pb3V, &QPushButton::clicked, this, &MainWindow::onpb3V);


    connect(ui->actionExit, &QAction::triggered, this, &QMainWindow::close);
    connect(ui->actionOptions, &QAction::triggered, m_settingsDialog, &QDialog::show);


    QString port = m_Settings->value("PORT").toString();
    if (!port.isEmpty())
    {
        //qDebug() << "TEST:" << port;
        ui->portEdit->setText(port);
        ui->l_VI->setText("VI:");

        // connect...
        // start timer.
        onConnectButtonClicked();

        m_Timer = new QTimer(this);
        connect(m_Timer, &QTimer::timeout, this, &MainWindow::onSecond);
        m_Timer->start(1000);


    }

}

void MainWindow::initRs232Modbus()
{
    if (modbusDevice) {
        modbusDevice->disconnectDevice();
        delete modbusDevice;
        modbusDevice = nullptr;
    }


    modbusDevice = new QModbusRtuSerialMaster(this);


    connect(modbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error)
    {
        statusBar()->showMessage(modbusDevice->errorString(), 5000);
    }
    );

    if (!modbusDevice) {
        ui->connectButton->setDisabled(true);

            statusBar()->showMessage(tr("Could not create Modbus master."), 5000);

    } else {
        connect(modbusDevice, &QModbusClient::stateChanged,
                this, &MainWindow::onModbusStateChanged);
    }
}

void MainWindow::onConnectButtonClicked()
{
    if (!modbusDevice)
        return;

    statusBar()->clearMessage();
    if (modbusDevice->state() != QModbusDevice::ConnectedState)
    {
        QString ap = ui->portEdit->text();
        if (!ap.isEmpty())
        {
            m_Settings->setValue("PORT", ap);
            qDebug() << "write PORT on open:" << ap;
        }

        // connect.
        modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, ui->portEdit->text());
        modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, m_settingsDialog->settings().parity);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,  m_settingsDialog->settings().baud);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,m_settingsDialog->settings().dataBits);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, m_settingsDialog->settings().stopBits);
        modbusDevice->setTimeout(m_settingsDialog->settings().responseTime);
        modbusDevice->setNumberOfRetries(m_settingsDialog->settings().numberOfRetries);
        if (!modbusDevice->connectDevice()) {
            statusBar()->showMessage(tr("Connect failed: ") + modbusDevice->errorString(), 5000);
        } else {
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
        }
    } else {
        m_Timer->stop();
        delete m_Timer;
        m_Timer=nullptr;
        modbusDevice->disconnectDevice();
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
    }
}

void MainWindow::onModbusStateChanged(int state)
{
    bool connected = (state != QModbusDevice::UnconnectedState);
    ui->actionConnect->setEnabled(!connected);
    ui->actionDisconnect->setEnabled(connected);

    if (state == QModbusDevice::UnconnectedState)
        ui->connectButton->setText(tr("Connect"));
    else if (state == QModbusDevice::ConnectedState)
        ui->connectButton->setText(tr("Disconnect"));
}

void MainWindow::onpb3V()
{
    onSetUint(0,1300);
}

void MainWindow::onpb2V()
{
    onSetUint(0,100);
  //  onSetUint(9,1);  // ON

}

void MainWindow::onSetUint(int no, int data)
{
    qDebug() << "onSetUint " << no << " data: " << data;
    if (!modbusDevice)
        return;

    const auto table = QModbusDataUnit::RegisterType::HoldingRegisters;

    quint16 numberOfEntries = 1;
    QModbusDataUnit writeUnit(table, no, numberOfEntries);

    quint16 xx= data;
    qDebug() << " add: " << no << " data: " << xx;
    writeUnit.setValue(0, xx);

    if (auto *reply = modbusDevice->sendWriteRequest(writeUnit, serverEdit)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                        .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                        5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
                        arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
        statusBar()->showMessage(tr("Write error: ") + modbusDevice->errorString(), 5000);
    }

}

void MainWindow::onReadDPS()
{
    if (!modbusDevice)
        return;

    statusBar()->clearMessage();

    const auto table =QModbusDataUnit::RegisterType::HoldingRegisters;

    if (auto *reply = modbusDevice->sendReadRequest(QModbusDataUnit(table, 0, 12), serverEdit)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &MainWindow::onReadDps);
        else
            delete reply; // broadcast replies return immediately
    } else {
        statusBar()->showMessage(tr("Read error: ") + modbusDevice->errorString(), 5000);
    }
}


void MainWindow::onReadDps()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        for (int i = 0, total = int(unit.valueCount()); i < total; ++i)
        {
            int no = unit.startAddress() + i;
            int data = unit.value(i);
            const QString entry = tr("Address: %1, Value: %2").arg(no)
                                     .arg(QString::number(data,
                                          unit.registerType() <= QModbusDataUnit::Coils ? 10 : 16));

            dpsData.setValue(no,data);
        }

        updateData();


    } else if (reply->error() == QModbusDevice::ProtocolError) {
        statusBar()->showMessage(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->rawResult().exceptionCode(), -1, 16), 5000);
    } else {
        statusBar()->showMessage(tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, 16), 5000);
    }

    reply->deleteLater();
}

void MainWindow::onReadReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
            const QString entry = tr("Address: %1, Value: %2").arg(unit.startAddress() + i)
                                     .arg(QString::number(unit.value(i),
                                          unit.registerType() <= QModbusDataUnit::Coils ? 10 : 16));
            //ui->readValue->addItem(entry);
        }
    } else if (reply->error() == QModbusDevice::ProtocolError) {
        statusBar()->showMessage(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->rawResult().exceptionCode(), -1, 16), 5000);
    } else {
        statusBar()->showMessage(tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, 16), 5000);
    }

    reply->deleteLater();
}


void MainWindow::onSecond()
{
   if (modbusDevice->state() == QModbusDevice::ConnectedState)
   {
       //qDebug() << "MainWindow::OnSecond connected";
       onReadDPS();
       return;
   }
   qDebug() << "MainWindow::OnSecond but closed -> stopping";

   //m_Timer->stop();

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
    ui->lcdVI->display(VI);
    ui->lcdVI->setStyleSheet("background: black; color: #008080");

    ui->lcdV->setDigitCount(4);
    ui->lcdV->display(VO);
    ui->lcdV->setStyleSheet("background: black; color: green");

    ui->lcdIO->setDigitCount(4);
    ui->lcdIO->display(IO);
    ui->lcdIO->setStyleSheet("background: black; color: yellow");

   // ui->lcdV->display(423);
   // ui->l_VI->setText(vi);

}

