#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "scanner.h"

#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // set buttons color
    ui->inputButton->setStyleSheet("background-color: rgb(230, 230, 230); color: #000000;");
    ui->exitButton->setStyleSheet("background-color: rgb(230, 230, 230); color: #000000;");
    // set browsing files button
    connect(ui->inputButton, &QPushButton::clicked, this, [this] {
        QString fileName = QFileDialog::getOpenFileName(this, "Select File");
        if (!fileName.isEmpty()) {
            QFileInfo fileInfo(fileName);
            QString name = fileInfo.fileName();
            ui->infoLabel->setText("Name: " + name + "\n");

            auto data = scanFile(fileName.toStdString());
            for (auto &pair : data) {
                ui->infoLabel->setText(ui->infoLabel->text() + QString::fromStdString(pair.first + ": " + pair.second) + "\n");
            }
        }
    });
    // set exit button
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::on_exitButton);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exitButton() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}
