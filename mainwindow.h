#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void convertRSTtoHTML();
    void open();
    void save();
    void setEditing();
    void toggleAutoConvert(bool checked);

private:

    Ui::MainWindow *ui;

    QString currentFileName;
    QFile *currentFile;
    QString readFile(QFile *file) const;
    bool saved;
    void writeFile(QFile *file, const QString& text);
    void setWindowCurrentFileName(const QString& name);
};

#endif // MAINWINDOW_H
