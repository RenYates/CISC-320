#ifndef APPOINTMENT_H
#define APPOINTMENT_H


#include <string>
#include <QSqlDatabase>
#pragma once
using namespace std;

class appointment
{
public:
    // constructor for appointment class
    appointment();
    appointment(long int ID, int day, int month, int year, long int studentNum, string doctor, string time, bool refill, bool pending, string reminderText, bool reminder);

    void appointmentReminder();
    bool deleteAppointment(); // returns true if deleted, false if error
    void pendingApointment();

    // getters for appointment class
    long int getID();
    int getDay();
    int getMonth();
    int getYear();
    string getTime();
    long int getStudentNum();
    string getDoctor();
    bool getRefill();
    bool getPending();
    bool getReminder();
    string getReminderText();

    // setters for appointment class
    void setID(long int aptID);
    void setDay(int intDay);
    void setMonth(int intMonth);
    void setYear(int aptYear);
    void setTime(string aptTime);
    void setStudentNum(long int studentNum);
    void setDoctor(string aptDoctor);
    void setRefil(bool aptRefill);
    void setPending(bool aptPending);
    void setReminder(bool aptReminder);

private:
    long int convertTimeToMin(int hour, int minute);
    void convertMinToTime(int min,int &hours, int &minutes);
    long int id;
    int day;
    int month;
    int year;
    long int studentNum;
    string doctor;
    string time;
    bool refill;
    bool pending;
    string txtReminder;
    bool reminder;
};

#endif // APPOINTMENT_H
