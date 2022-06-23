#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "fileviewer.h"

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    FileViewer* fileViewer;
};
#endif // MAINWINDOW_H
