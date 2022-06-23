#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      fileViewer(new FileViewer(this))
{
    this->setMinimumSize(800, 600);
}

MainWindow::~MainWindow()
{
    if(fileViewer) delete fileViewer;
}

