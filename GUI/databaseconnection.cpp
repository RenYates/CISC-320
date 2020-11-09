#include "databaseconnection.h"
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

DatabaseConnection::DatabaseConnection()
{

}

void DatabaseConnection::DatabaseConnect(){
    const QString DRIVER("QSQLITE");

    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);

        db.setDatabaseName("C:/Users/laure/OneDrive/Documents/Queen's Courses/CISC 320/QHealth Repository/qhealth.db");

        if(!db.open())
            qWarning() << "MainWindow::DatabaseConnect - ERROR: " << db.lastError().text();
    }
    else
        qWarning() << "MainWindow::DatabaseConnect - ERROR: no driver " << DRIVER << " available";
}

void DatabaseConnection::DatabasePopulate()
{
    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON;"); // Making sure foreign keys are turned on before other code execution

    if(!query.exec("INSERT INTO clinician(username, email) VALUES('admin123', 'password123')"))
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
    if(!query.exec("INSERT INTO students(firstName, lastName, studentID, email) VALUES('Admin', 'Admin', 123123123, 'admin@queensu.ca')"))
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
}

