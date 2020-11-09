#include "clinician.h"
#include <string>
#include "appointment.h"
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDate>

using namespace std;
//clinic class

//will add attributes to the instance of the clinician
//only once it is confirmed that the clinician is in the database
clinician::clinician(){

}
clinician::clinician(string clinicianID, string clinicianEmail, bool clinAdmin){
    username= clinicianID;
    email = clinicianEmail;
    admin = clinAdmin;
}

//updates the status of the appointment in the database
void clinician::bookStudentAppointment(appointment apt){
    apt.pendingApointment();
    confirmAppointment(apt);
}

//view list of all upcoming appointments in the database
QList<QStringList> clinician::viewUpcomingAppointments(){
    QList<QStringList> upcomingApts; // 2D QStringList of all past appointments
    QSqlQuery query;
    QDate today = QDate::currentDate();
    string date = to_string(today.year()) + "/" + to_string(today.month()) + "/" + to_string(today.day());
    query.prepare("SELECT * FROM appts WHERE pending != 1 AND appDate >= :today AND studentID IS NOT NULL");
    query.bindValue(":today", date.c_str());
    if(!query.exec()){
        qWarning() << "Clinician::viewUpcomingAppointments - ERROR: " << query.lastError().text();
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
            upcomingApts.append(aptInfo);
        }
    }
    return upcomingApts;

}

//view list of all pending appointents in the database
QList<QStringList> clinician::viewPendingAppointments(){
    QList<QStringList> pendingApts;
    QSqlQuery query;
    QDate today = QDate::currentDate();
    string date = to_string(today.year()) + "/" + to_string(today.month()) + "/" + to_string(today.day());
    query.prepare("SELECT * FROM appts WHERE pending = 1 AND appDate >= :today AND studentID IS NOT NULL");
    query.bindValue(":today", date.c_str());
    if(!query.exec()){
        qWarning() << "Clinician::viewPendingAppointments - ERROR: " << query.lastError().text();
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
            pendingApts.append(aptInfo);
        }
    }
    return pendingApts;

}

//send an appointment email once the appointment is booked
// this method can be called from the bookStudentAppointment or the button
void clinician::confirmAppointment(appointment apt){
    long idNum = apt.getID();
    QSqlQuery query;

    query.prepare("UPDATE appts SET pending = false WHERE apptID = :idNum");
    query.bindValue(":idNum", to_string(idNum).c_str());
    if(!query.exec()){
        qWarning() << "MainWindow::DatabaseConfirmApt - ERROR: " << query.lastError().text();
    }
}

//updates the Queue Code which connects with the Queue class
void clinician::updateQueueCode(){

}

//connects with appointment class and deletes appointment from database
void clinician::cancelStudentAppointment(appointment apt){
    apt.deleteAppointment();
}

//changes the questionnaire questions
void clinician::changeQuestionnaire(){

}

//views questionnaire answers
void clinician::viewQuestionnaire(){

}

void clinician::setUsername(string clinicianID){
    username= clinicianID;
}
void clinician::setEmail(string clinEmail){
    email = clinEmail;
}
void clinician::setAdmin(bool clinAdmin){
    admin = clinAdmin;
}
string clinician::getUsername(){
    return username;
}
string clinician::getEmail(){
    return email;
}
bool clinician::getAdmin(){
    return admin;
}
