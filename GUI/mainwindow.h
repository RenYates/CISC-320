#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QStyleFactory>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QtSql>
#include "queue.h"
#include "student.h"
#include "appointment.h"
#include "clinician.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_clinicianBookAppointment_clicked();

    void upcomingAppointmentButtonClick(int button);

    void stdUpcomingAppointmentButtonClick();

    void pendingAppointmentButtonClick(int button);

    void on_clinicianToolboxCollapse_clicked();

    void on_clinicianUpcomingAppointmentSendReminder_clicked();

    void on_clinicianPendingAppointmentSubmit_clicked();

    void on_clinicianBookAppointmentSubmit_clicked();

    void on_clinicianUpdateWalkInCode_clicked();

    void on_studentButton_clicked();

    void on_clinicianButton_clicked();

    void on_clinicianLogin_clicked();

    void on_ClinicianLogout_clicked();

    void on_studentLogin_clicked();

    void on_studentLogout_clicked();

    void on_studentBookAppointmentSubmit_clicked();

    void on_studentBookAppointment_clicked();

    void on_studentToolboxCollapse_clicked();

    void on_joinQueueButton_clicked();

    void on_studentCancelAppointment_clicked();

    void on_studentNoThanks_clicked();

    void on_studentQuestionnaireButton_clicked();

    void on_studentQuestionnaireSubmit_clicked();

    void on_studentOptionalQuestionnaireButton_clicked();

    void on_studentPostBookingFinish_clicked();

    void on_studentQuestionnaireButtonFromQueue_clicked();

    void on_studentNoThanksFromQueue_clicked();

    void clock();

    void on_viewApptHistory_clicked();

    void on_buttonHome_clicked();

    void on_removeStudentFromQueue_clicked();

    void on_clinicianCheckQuestionnaire_clicked();

private:
    queue patientQueue;
    student currentStudent;
    appointment currentAppointment;
    clinician currentClinician;
    QList<appointment> clinicianUpcomingApts;
    QList<appointment> clinicianPendingApts;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
