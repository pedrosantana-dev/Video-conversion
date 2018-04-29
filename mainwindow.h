#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QScrollBar>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QProcess * process;
    QString outputText;
    QString fileName;
    QString outputFileName;

private slots:
    void on_abrirBtn_clicked();
    void on_converterBtn_clicked();

    void processStarted();
    void readyReadStandardOutput();
    void processFinished();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
