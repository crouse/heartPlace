#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QTableView>
#include <QFileDialog>
#include <QFile>
#include <QDate>
#include <QStandardPaths>
#include <QSqlQuery>
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

    QSqlDatabase db;
    QSqlTableModel *model;
    QString dbLocation;
    void setModel(QString filter);
    void insertRec();

    void setDb();
private slots:
    void on_pushButton_clicked();

    void on_actionreport_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
