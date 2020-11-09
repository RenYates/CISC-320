#ifndef STUDENT_H
#define STUDENT_H
#include <string>
#include "appointment.h"
#pragma once
using namespace std;

//student class declaration file
class student
{
public:
    student();
    student(string stdEmail, long stdNum, string stdFirst, string stdLast);
    QList<QStringList> viewHistory();
    void createAccount();
    QList<QStringList> viewUpcomingAppointment();
    void viewPlaceNumberInQueue();
    void createAppointment();
    void cancelAppointment(appointment apt);
    void joinQueue();
    void setEmail(string inputEmail);
    void setStudentNumber(long inputStudentNumber);
    void setFirstName(string inputFirstName);
    void setLastName(string inputLastName);
    string getEmail();
    long getStudentNumber();
    string getFirstName();
    string getLastName();

private:
    string email;
    long studentNumber;
    string firstName;
    string lastName;
};

#endif // STUDENT_H
