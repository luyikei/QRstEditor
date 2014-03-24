#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QMessageBox>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentFileName("Untitled"),
    saved(true)
{
    ui->setupUi(this);

    connect(ui->Convert, SIGNAL(clicked()), this, SLOT(convertRSTtoHTML()));
    connect(ui->ReSTEdit, SIGNAL(textChanged()), this, SLOT(convertRSTtoHTML()));
    connect(ui->ReSTEdit, SIGNAL(textChanged()), this, SLOT(setEditing()));
    connect(ui->IsAutoConvert, SIGNAL(toggled(bool)), this, SLOT(toggleAutoConvert(bool)));
    connect(ui->openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->closeAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->toEmphasisAction, SIGNAL(triggered()), ui->ReSTEdit, SLOT(toEmphasis()) );
    connect(ui->toStrongAction, SIGNAL(triggered()), ui->ReSTEdit, SLOT(toStrong()) );
    setWindowCurrentFileName(currentFileName);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::convertRSTtoHTML()
{
    QProcess p;
    p.start("pandoc -f rst -t html");
    p.write(ui->ReSTEdit->toPlainText().toLatin1());
    p.closeWriteChannel();
    p.waitForFinished(-1);

    QString p_stdout = p.readAllStandardOutput();\
    ui->HTMLView->setHtml(p_stdout);
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open Rst File",
                                                    QDir::homePath(),
                                                    "ReStructuredText Files (*.rst *.rest)"
                                                    );
    if(fileName == "")
        return;
    currentFile = new QFile(fileName);
    ui->ReSTEdit->setPlainText(readFile(currentFile));
    currentFileName=currentFile->fileName();
    setWindowCurrentFileName(currentFileName);
}

void MainWindow::save()
{
    if(!saved){
        if(currentFileName == "Untitled"){
            currentFile = new QFile(QFileDialog::getSaveFileName(this,
                                                       "Save Rst File",
                                                       QDir::homePath(),
                                                       "ReStructuredText Files (*.rst *.rest)"
                                                       )
                                    );
            currentFileName=currentFile->fileName();
            setWindowCurrentFileName(currentFileName);
        }
        writeFile(currentFile, ui->ReSTEdit->toPlainText());
    }
}

void MainWindow::setEditing()
{
    saved = false;
    setWindowCurrentFileName(currentFileName+"*");
}

void MainWindow::toggleAutoConvert(bool checked)
{
    if(checked)
        connect(ui->ReSTEdit, SIGNAL(textChanged()), this, SLOT(convertRSTtoHTML()));
    else
        disconnect(ui->ReSTEdit, SIGNAL(textChanged()), this, SLOT(convertRSTtoHTML()));

}

QString MainWindow::readFile(QFile *file) const
{
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    QTextStream in(file);

    return in.readAll();

}

void MainWindow::writeFile(QFile *file, const QString& text)
{
    if (file->open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(file);
        out << text;
    }
}

void MainWindow::setWindowCurrentFileName(const QString& name)
{
    setWindowTitle(QString("%1 - QRstEditor").arg(name));
}
