#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setDb();
    this->setWindowIcon(QIcon(":/heartPlace.icns"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::insertRec()
{
    QString name = ui->lineEditName->text().trimmed();
    QString gender = ui->lineEditGender->text().trimmed().toUpper();
    QString phone = ui->lineEditPhone->text().trimmed();
    QString birthday = ui->lineEditBirth->text().trimmed();
    QString logdate = ui->lineEditLogDate->text().trimmed();
    QString currentDt = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    // 判断名字和日期是否为空，如果为空，则跳转回输入框
    if (name.isEmpty()) {
        ui->lineEditName->setFocus();
        return;
    }

    if (logdate.isEmpty()) {
        ui->lineEditLogDate->setFocus();
        return;
    }

    if (!phone.isEmpty() && phone.length() != 11) {
        ui->lineEditPhone->setFocus();
        return;
    }

    QSqlQuery query;
    QString insert = QString("insert into heart_place (name, gender, phone, birthday, logdate, current) "
                             " values ('%2', '%3', '%4', '%5', '%6', '%7');"
                             ).arg(name).arg(gender).arg(phone).arg(birthday).arg(logdate).arg(currentDt);
    query.exec(insert);
    db.commit();

    model->setSort(5,Qt::DescendingOrder);
    model->select();

    ui->tableView->reset();
}

void MainWindow::setModel(QString filter)
{
    model = new QSqlTableModel(this);
    model->setTable("heart_place");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setSort(0, Qt::AscendingOrder);
    model->setHeaderData(0, Qt::Horizontal, "姓名");
    model->setHeaderData(1, Qt::Horizontal, "性别");
    model->setHeaderData(2, Qt::Horizontal, "手机");
    model->setHeaderData(3, Qt::Horizontal, "生日");
    model->setHeaderData(4, Qt::Horizontal, "签到日期");
    model->setHeaderData(5, Qt::Horizontal, "记录时间");
    model->setHeaderData(6, Qt::Horizontal, "其他");

    ui->tableView->setModel(model);
    ui->tableView->alternatingRowColors();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    model->setFilter(filter);
    model->select();
    ui->tableView->reset();
}

void MainWindow::setDb()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    dbLocation = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    qDebug() << "data path" << dbLocation;

    db.setDatabaseName(QString("%1/sign.db").arg(dbLocation));

    if (!db.open()) {
        qDebug() << "Can not open sign.db";
        QMessageBox::information(this, "", "无法连接数据库");
    } else {
        QSqlQuery query;
        QString createTableSql = QString("Create table heart_place ("
                                         " name varchar(32), "
                                         " gender varchar(4), "
                                         " phone varchar(32), "
                                         " birthday datetime, "
                                         " logdate datetime,"
                                         " current datetime,"
                                         " note varchar(128) "
                                         ");");

        QString indexTableSql = QString("Create table heart_place_dict ( "
                                        " name varchar(32), "
                                        " gender varchar(32),"
                                        " phone varchar(32),"
                                        " birthday datetime,"
                                        " logdate datetime"
                                        " );");

        query.exec(createTableSql);
        query.exec(indexTableSql);

        qDebug() << createTableSql << indexTableSql;
        db.commit();
    }
    setModel("");
}

void MainWindow::on_pushButton_clicked()
{
    insertRec();
    ui->lineEditName->clear();
    ui->lineEditBirth->clear();
    ui->lineEditPhone->clear();
    ui->lineEditGender->clear();
    ui->lineEditLogDate->clear();
    ui->lineEditName->setFocus();
}

void MainWindow::on_actionreport_triggered()
{

}
