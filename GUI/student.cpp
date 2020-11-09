#include "student.h"
#include <string>
#include "appointment.h"
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <vector>
#include <QDate>
#include <QTime>
#include <QList>

/*
 * Student class
 */
//keeping in mind our attributes
//string email;
//long studentNumber;
//string firstName;
//string lastName;

using namespace std;

student::student(){

}
//student constructor to fill in our attributes
//will connect with checking of the login page class and then will add in the attributes
student::student(string stdEmail, long stdNum, string stdFirst, string stdLast){
    email = stdEmail;
    studentNumber = stdNum;
    firstName = stdFirst;
    lastName = stdLast;
}
//view appointment history
//this method should interact with the past appointment table
QList<QStringList> student::viewHistory(){
    QList<QStringList> history; // 2D QStringList of all past appointments
    QSqlQuery query;
    QDate today = QDate::currentDate();
    string date = to_string(today.year()) + "/" + to_string(today.month()) + "/" + to_string(today.day());
    query.prepare("SELECT * FROM appts WHERE studentID = :stdID AND appDate < :today");
    query.bindValue(":stdID", to_string(this->studentNumber).c_str());
    query.bindValue(":today", date.c_str());
    if(!query.exec()){
        qWarning() << "MainWindow::DatabaseDelete - ERROR: " << query.lastError().text();
    }
    else{
        while(query.next()){
            QString doctor = query.value(1).toString();
            QString time = query.value(2).toString();
            QString date = query.value(3).toString();
            QString stdID = query.value(4).toString();
            QString refill = query.value(5).toString();
            QString pending = query.value(6).toString();
            QString reminder = query.value(7).toString();
            QStringList aptInfo = {doctor, time, date, stdID, refill, pending, reminder};
            history.append(aptInfo);
        }
    }
    // use 2D list history containing appointment info for all appointments after today's date to display in UI
    return history;
}
//will connect with the login page and the class that checks to make sure that the student has an acccount or not
void student::createAccount(){

}
//connects with the database or appointment class
QList<QStringList> student::viewUpcomingAppointment(){
    QList<QStringList> upcomingApt; // 2D QStringList of all past appointments
    QSqlQuery query;
    QDate today = QDate::currentDate();
    string studNumber = to_string(studentNumber);
    string date = to_string(today.year()) + "/" + to_string(today.month()) + "/" + to_string(today.day());
    query.prepare("SELECT * FROM appts WHERE studentID = :number AND appDate >= :today");
    query.bindValue(":today", date.c_str());
    query.bindValue(":number", studNumber.c_str());
    if(!query.exec()){
        qWarning() << "MainWindow::DatabaseDelete - ERROR: " << query.lastError().text();
    }
    else{
        while(query.next()){
            QString aptNum = query.value(0).toString();
            QString doctor = query.value(1).toString();
            QString time = query.value(2).toString();
            QString date = query.value(3).toString();
            QString stdID = query.value(4).toString();
            QString refill = query.value(5).toString();
            QString pending = query.value(6).toString();
            QString reminder = query.value(7).toString();
            QStringList aptInfo = {aptNum, doctor, time, date, stdID, refill, pending, reminder};
            upcomingApt.append(aptInfo);
        }
    }
    return upcomingApt;
};
//connects with the queue class
void student::viewPlaceNumberInQueue(){

};
//connects with the appointment class
void student::createAppointment(){

};
//connects with the appointment class
void student::cancelAppointment(appointment apt){
    apt.deleteAppointment();
};
//connects with the queue class
void student::joinQueue(){

};

void student::setEmail(string inputEmail) {
    email = inputEmail;
}

void student::setLastName(string inputLastName) {
    lastName = inputLastName;
}

void student::setFirstName(string inputFirstName) {
    firstName = inputFirstName;
}

void student::setStudentNumber(long inputStudentNumber) {
    studentNumber = inputStudentNumber;
}

string student::getEmail() {
    return email;
}

string student::getFirstName() {
    return firstName;
}

string student::getLastName() {
    return lastName;
}

long student::getStudentNumber() {
    return studentNumber;
}

