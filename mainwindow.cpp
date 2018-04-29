#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    process = new QProcess(this);
    connect(process, SIGNAL(started()), this,
            SLOT(processStarted()));
    connect(process, SIGNAL(readyReadStandardOutput()), this,
            SLOT(readyReadStandardOutput()));
    connect(process, SIGNAL(finished(int)), this,
            SLOT(processFinished()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_abrirBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Abrir Video"),
                                                    "", "Arquivos de video (*.avi *.mp4 *.mov)");
    ui->lineEdit->setText(fileName);
}

void MainWindow::on_converterBtn_clicked()
{
    QString ffmpeg = "C:/ffmpeg/bin/ffmpeg";
    QStringList argumentos;

    fileName = ui->lineEdit->text();
    if (fileName != "")
    {
        QFileInfo fileInfo = QFile(fileName);
        outputFileName = fileInfo.path() + "/" + fileInfo.completeBaseName();

        if (QFile::exists(fileName))
        {
            // 0 - AVI
            // 1 - MP4
            // 2 - MOV
            int format = ui->comboBox->currentIndex();
            if (format == 0)
            {
                outputFileName += ".avi";
            }
            else if (format == 1)
            {
                outputFileName += ".mp4";
            }
            else if (format == 2)
            {
                outputFileName += ".mov";
            }

            qDebug() << outputFileName << format;

            argumentos << "-i" << fileName << outputFileName;

            qDebug() << argumentos;

            process->setProcessChannelMode(QProcess::MergedChannels);
            process->start(ffmpeg, argumentos);
        }
        else
        {
            QMessageBox::warning(this, "Falhou", "Falhou ao abrir arquivo de video.");
        }
    }
    else
    {
        QMessageBox::warning(this, "Falhou", "Arquivo não selecionado!");
    }
}

void MainWindow::processStarted()
{
    qDebug() << "Processo iniciado.";
    ui->abrirBtn->setEnabled(false);
    ui->comboBox->setEditable(false);
    ui->converterBtn->setEnabled(false);
}

void MainWindow::readyReadStandardOutput()
{
    outputText += process->readAllStandardOutput();
    ui->textEdit->setText(outputText);

    ui->textEdit->verticalScrollBar()->setSliderPosition(ui->textEdit->verticalScrollBar()->maximum());
}

void MainWindow::processFinished()
{
    qDebug() << "Processo terminado.";

    if (QFile::exists(outputFileName))
    {
        QMessageBox::information(this, "Sucesso", "Video convertido com sucesso!");
    }
    else
    {
        QMessageBox::information(this, "Falhou", "Falhou ao converter video.");
    }

    ui->abrirBtn->setEnabled(true);
    ui->comboBox->setEditable(true);
    ui->converterBtn->setEnabled(true);
}
