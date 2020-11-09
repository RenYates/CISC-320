#include "appointment.h"
#include <cmath>
#include <string>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <iostream>
using namespace std;
appointment::appointment(){};
// constructor for appointment class
appointment::appointment(long int aptID, int aptDay, int aptMonth, int aptYear, long int stdNum, string doc, string aptTime, bool aptRefill, bool aptPending, string reminderText, bool aptReminder){
    // will add error checking as needed, this is just base instantiation
    id = aptID;
    day = aptDay;
    month = aptMonth;
    year = aptYear;
    studentNum = stdNum;
    doctor = doc;
    time = aptTime; // in minutes, use convert function when reading from db to convert time to minutes
    refill = aptRefill;
    pending = aptPending;
    txtReminder = reminderText;
    reminder = aptReminder;
}

// appointment class methods, interacts with database

// sends appointment reminder email to student (info pulled from database)
void appointment::appointmentReminder(){

}
// deletes selected appointment from database (sets student# to 0 for
bool appointment::deleteAppointment(){
    //accesses value within the memory address of the appointment object being referenced
    long idNum = this->id;
    QSqlQuery query;

    query.prepare("UPDATE appts SET studentID = NULL, refill = NULL, pending = true, reminder = NULL, reminderB = false WHERE apptID = :idNum");
    query.bindValue(":idNum", to_string(idNum).c_str());
    if(!query.exec()){
        qWarning() << "MainWindow::DatabaseDelete - ERROR: " << query.lastError().text();
        return false;
    }
    else
        return true;

} // returns true if deleted, false if error

// inserts appointment into database as pending appointment
void appointment::pendingApointment(){
    //accesses value within the memory address of the appointment object being referenced
    long int idNum = this->id;
    long int stdNum = this->studentNum;
    string doc = this->doctor;
    bool aptRefill = this->refill;
    QSqlQuery query;
    // instead of inserting to table, update values in table
    //query.prepare("INSERT INTO appts(apptID, doctor, apptTime, appDate, studentID, refill, pending, reminder, reminderB) VALUES(:idNum, :doc, :aptTime, :aptDate, :stdNum, :aptRefill, :aptPending, :txtReminder, :aptReminder)");
    query.prepare("UPDATE appts SET studentID = :stdNum, refill = :aptRefill, pending = true, reminderB = false WHERE apptID = :idNum");
    query.bindValue(":idNum", to_string(idNum).c_str());
    query.bindValue(":stdNum", to_string(stdNum).c_str());
    query.bindValue(":aptRefill", to_string(aptRefill).c_str());
    if(!query.exec())
        qWarning() << "MainWindow::DatabasePendingApt - ERROR: " << query.lastError().text();
}

long int appointment::convertTimeToMin(int hour, int minute){
    return (hour*60) + minute;
}

void appointment::convertMinToTime(int min, int &hours, int &minutes){
    hours = int(floor(min/60));
    minutes = min%60;
}

// getters for appointment class
long int appointment::getID(){
    return id;
}
int appointment::getDay(){
    return day;
}
int appointment::getMonth(){
    return month;
}
int appointment::getYear(){
    return year;
}
string appointment::getTime(){
    return time;
}
long int appointment::getStudentNum(){
    return studentNum;
}
string appointment::getDoctor(){
    return doctor;
}
bool appointment::getRefill(){
    return refill;
}
bool appointment::getPending(){
    return pending;
}
bool appointment::getReminder(){
    return reminder;
}

// setters for appointment class
void appointment::setID(long int aptID){
    id = aptID;
}
void appointment::setDay(int aptDay){
    day =  aptDay;
}
void appointment::setMonth(int aptMonth){
    month = aptMonth;
}
void appointment::setYear(int aptYear){
    year = aptYear;
}
void appointment::setTime(string aptTime){
    time = aptTime;
}
void appointment::setStudentNum(long int aptStdNum){
    studentNum = aptStdNum;
}
void appointment::setDoctor(string aptDoctor){
    doctor = aptDoctor;
}
void appointment::setRefil(bool aptRefill){
    refill = aptRefill;
}
void appointment::setPending(bool aptPending){
    pending = aptPending;
}
void appointment::setReminder(bool aptReminder){
    reminder = aptReminder;
}

