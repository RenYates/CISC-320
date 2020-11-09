#ifndef CLINICIAN_H
#define CLINICIAN_H

#include <string>
#include "appointment.h"
#pragma once
using namespace std;
class clinician
{
public:
    clinician();
    clinician(string username, string email, bool admin);
    string username;
    string email;
    bool admin;
    void setUsername(string clinicianID);
    void setEmail(string clinEmail);
    void setAdmin(bool clinAdmin);
    string getUsername();
    string getEmail();
    bool getAdmin();
    void createAccount();
    void bookStudentAppointment(appointment apt);
    QList<QStringList> viewUpcomingAppointments();
    QList<QStringList> viewPendingAppointments();
    void confirmAppointment(appointment apt);
    void updateQueueCode();
    void cancelStudentAppointment(appointment apt);
    void changeQuestionnaire();
    void viewQuestionnaire();

};

#endif // CLINICIAN_H
