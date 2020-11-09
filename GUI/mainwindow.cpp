#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databaseconnection.h"
#include "databaseconnection.cpp"
#include "student.h"
#include "clinician.h"
#include <iostream>
#include <string>
#include "queue.h"
#include <QList>
#include <QString>
#include "clinician.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->setStyle(QStyleFactory::create("Fusion"));

    ui->clinicianBookAppointmentFrame->setHidden(true);
    ui->clinicianPendingAppointmentFrame->setHidden(true);
    ui->clinicianUpcomingAppointmentFrame->setHidden(true);
    ui->clinicianToolboxCollapse->setHidden(true);

    ui->studentBookAppointmentFrame->setHidden(true);
    ui->studentToolboxCollapse->setHidden(true);
    ui->studentAppointmentConfirmantionFrame->setHidden(true);

    /***********************************Starting the clock************************************/
    QTimer *timer = new QTimer(this);
    QString temp = "";
    connect(timer, SIGNAL(timeout()), this, SLOT(clock()));
    timer->start(60000);
    clock();
    /****************************************************************************/
    /*******************************The following code is just to fill the ui*****************************************/


        /**********clinician upcoming List***************/

        DatabaseConnection dc; //creating a global database connection
        dc.DatabaseConnect();
        //dc.DatabasePopulate();

        //adding doctors
        ui->viewDoctor->addItem("Dr. A");
        ui->viewDoctor->addItem("Dr. B");
        ui->viewDoctor->addItem("Dr. C");
        ui->viewDoctor->addItem("Dr. X");
        ui->viewDoctor->addItem("Dr. Y");
        ui->viewDoctor->addItem("Dr. Z");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_clinicianBookAppointment_clicked()
{
    if(ui->clinicianBookAppointmentFrame->isHidden())
    {
        ui->clinicianUpcomingAppointmentFrame->setHidden(true);
        ui->clinicianPendingAppointmentFrame->setHidden(true);
        ui->clinicianToolboxCollapse->setHidden(false);
        ui->clinicianBookAppointmentFrame->setHidden(false);
    }
    else {

        ui->clinicianBookAppointmentFrame->setHidden(true);
    }
}

void MainWindow::on_clinicianToolboxCollapse_clicked()
{
    ui->clinicianUpcomingAppointmentFrame->setHidden(true);
    ui->clinicianPendingAppointmentFrame->setHidden(true);
    ui->clinicianBookAppointmentFrame->setHidden(true);
    ui->clinicianToolboxCollapse->setHidden(true);
}

void MainWindow::on_clinicianUpcomingAppointmentSendReminder_clicked()
{
    int studentNumberDB = ui->reminderStudentNumber->text().toInt();
    QString reminder = ui->textBrowser->toPlainText();
    QSqlQuery queryAppointmentReminder;
    queryAppointmentReminder.prepare("UPDATE appts SET reminderB='1', reminder = :reminder WHERE studentID = :studentNumberDB");
    queryAppointmentReminder.bindValue(":studentNumberDB", studentNumberDB);
    queryAppointmentReminder.bindValue(":reminder", reminder);
    if (!queryAppointmentReminder.exec()){
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << queryAppointmentReminder.lastError().text();
    }
    QMessageBox::information(this,"Reminder","Reminder Has Been Sent To 'Student'");
}

void MainWindow::on_clinicianPendingAppointmentSubmit_clicked()
{

    int studentNumberDB = ui->pendingStudentNUmber->text().toInt();
    QSqlQuery queryAppointmentReminder;
    queryAppointmentReminder.prepare("UPDATE appts SET pending = '0' WHERE studentID = :studentNumberDB");
    queryAppointmentReminder.bindValue(":studentNumberDB", studentNumberDB);
    if (!queryAppointmentReminder.exec()){
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << queryAppointmentReminder.lastError().text();
    }
    QMessageBox::information(this,"Operation Successful","Appointment Confirmed!\nYou can view the appointment status in the appointment information");

    QList<QStringList> upcomingApts = currentClinician.viewUpcomingAppointments();
    if(!(upcomingApts.isEmpty())){
        QWidget *widget = new QWidget();
        ui->clinicianUpcomingAppointmentList->setWidget( widget );

        QVBoxLayout *layout = new QVBoxLayout();
        widget->setLayout( layout );

        clinicianUpcomingApts.clear();
        QSignalMapper *signalMapperUpcoming = new QSignalMapper(this);
        connect(signalMapperUpcoming, SIGNAL(mapped(int)), this, SLOT(upcomingAppointmentButtonClick(int)));

        for(int i=0; i<upcomingApts.length(); i++){
            //QStringList aptInfo = {aptNum, doctor, time, date, stdID, refill, pending, reminder};
            QString aptText = QString::number(i+1) + ". Student:" + upcomingApts[i][4] + "\nDoctor: " + upcomingApts[i][1] + "\nDate: " + upcomingApts[i][3] + "\nTime:" + upcomingApts[i][2];
            long int id = upcomingApts[i][0].toLong();
            string doctor = upcomingApts[i][1].toStdString();
            string time = upcomingApts[i][2].toStdString();
            QStringList dates = upcomingApts[i][3].split("/");
            int day = dates[2].toInt();
            int month = dates[1].toInt();
            int year = dates[0].toInt();
            long int stdNum = upcomingApts[i][4].toLong();
            bool refill = bool(upcomingApts[i][5].toInt());
            bool pending = bool(upcomingApts[i][6].toInt());
            bool reminderB = bool(upcomingApts[i][7].toInt());
            appointment upcoming(id, day, month, year, stdNum, doctor, time, refill, pending, "", reminderB);
            clinicianUpcomingApts.append(upcoming);
            QPushButton *button = new QPushButton(aptText);
            signalMapperUpcoming->setMapping(button, i);
            connect(button,SIGNAL(clicked()),signalMapperUpcoming,SLOT(map()));
            layout->addWidget( button );

            QWidget *wid = ui->clinicianPendingAppointmentList->widget();
            if(wid)
                wid->deleteLater();
        }
    }
    QList<QStringList> pendingApts = currentClinician.viewPendingAppointments();
    if(!(pendingApts.isEmpty())){
        ui->clinicianPendingAppointmentList->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
        ui->clinicianPendingAppointmentList->setWidgetResizable( true );

        QWidget *widget2 = new QWidget();
        ui->clinicianPendingAppointmentList->setWidget( widget2 );

        QVBoxLayout *layout2 = new QVBoxLayout();
        widget2->setLayout( layout2 );

        clinicianPendingApts.clear();
        QSignalMapper *signalMapperPending = new QSignalMapper(this);
        connect(signalMapperPending, SIGNAL(mapped(int)), this, SLOT(pendingAppointmentButtonClick(int)));
        for(int i=0; i<pendingApts.length(); i++){
            //QStringList aptInfo = {aptNum, doctor, time, date, stdID, refill, pending, reminder};
            QString aptText = QString::number(i+1) + ". Student:" + pendingApts[i][4] + "\nDoctor: " + pendingApts[i][1] + "\nDate: " + pendingApts[i][3] + "\nTime:" + pendingApts[i][2];
            long int id = pendingApts[i][0].toLong();
            string doctor = pendingApts[i][1].toStdString();
            string time = pendingApts[i][2].toStdString();
            QStringList dates = pendingApts[i][3].split("/");
            int day = dates[2].toInt();
            int month = dates[1].toInt();
            int year = dates[0].toInt();
            long int stdNum = stol(pendingApts[i][4].toStdString());
            bool refill = bool(pendingApts[i][5].toInt());
            bool pending = bool(pendingApts[i][6].toInt());
            bool reminderB = bool(pendingApts[i][7].toInt());
            appointment pendingApt(id, day, month, year, stdNum, doctor, time, refill, pending, "", reminderB);
            clinicianPendingApts.append(pendingApt);
            QPushButton *button = new QPushButton(aptText, widget2);
            signalMapperPending->setMapping(button, i);
            connect(button,SIGNAL(clicked()),signalMapperPending,SLOT(map()));
            layout2->addWidget( button );
        }
    }
}

void MainWindow::on_clinicianBookAppointmentSubmit_clicked()
{
    QMessageBox::information(this,"Operation Successful","Appointment Confirmed!\nThe student has been notified about this appointment");
}

void MainWindow::on_studentButton_clicked()
{
    ui->mainStack->setCurrentIndex(2);
    ui->studentStack->setCurrentIndex(0);

    ui->studentBookAppointmentFrame->setHidden(true);
    ui->studentToolboxCollapse->setHidden(true);
    ui->studentAppointmentConfirmantionFrame->setHidden(true);
    ui->studentPostBookingFrame->setHidden(true);

    //create a new student class
    student NewStudent;
}

void MainWindow::on_clinicianButton_clicked()
{
    ui->mainStack->setCurrentIndex(1);
    ui->clinicianStack->setCurrentIndex(0);

    //create a new clinician class
    clinician newClinician;
}

void MainWindow::on_clinicianLogin_clicked()
{
    QString userNameText = ui->clinicianUsername->text();
    QString emailText = ui->clinicianEmail->text();

    QSqlQuery queryClinicianLogin;

    queryClinicianLogin.prepare("SELECT userName, email FROM clinician WHERE username = :userNameText AND email= :emailText");
    queryClinicianLogin.bindValue(":userNameText",userNameText);
    queryClinicianLogin.bindValue(":emailText",emailText);

    if(!queryClinicianLogin.exec()) {
        //very wrong shouldn't get here
    }
    else if(!queryClinicianLogin.first()){
        ui->loginConfirmation->setText("Sorry, you are not in the Queen's Database");

    }
    else {
        ui->loginConfirmation->setText("Valid credentials");
        currentClinician.setUsername(userNameText.toStdString());
        currentClinician.setEmail(emailText.toStdString());
        currentClinician.setAdmin(true);
        ui->clinicianStack->setCurrentIndex(1);
        if (patientQueue.getSize() == 0) {
            ui->nextStudentInQueue->setText("No students waiting");
        }
        else {
            QString nextStudentText = QString::fromStdString(patientQueue.top().getFirstName() + patientQueue.top().getLastName());
            ui->nextStudentInQueue->setText(nextStudentText);
        }
    }
    ui->clinicianUpcomingAppointmentList->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    ui->clinicianUpcomingAppointmentList->setWidgetResizable( true );

    QList<QStringList> upcomingApts = currentClinician.viewUpcomingAppointments();
    if(!(upcomingApts.isEmpty())){
    QWidget *widget = new QWidget();
    ui->clinicianUpcomingAppointmentList->setWidget( widget );

    QVBoxLayout *layout = new QVBoxLayout();
    widget->setLayout( layout );
    clinicianUpcomingApts.clear();

    QSignalMapper *signalMapperUpcoming = new QSignalMapper(this);
    connect(signalMapperUpcoming, SIGNAL(mapped(int)), this, SLOT(upcomingAppointmentButtonClick(int)));

    for(int i=0; i<upcomingApts.length(); i++){
        //QStringList aptInfo = {aptNum, doctor, time, date, stdID, refill, pending, reminder};
        QString aptText = QString::number(i+1) + ". Student:" + upcomingApts[i][4] + "\nDoctor: " + upcomingApts[i][1] + "\nDate: " + upcomingApts[i][3] + "\nTime:" + upcomingApts[i][2];
        long int id = upcomingApts[i][0].toLong();
        string doctor = upcomingApts[i][1].toStdString();
        string time = upcomingApts[i][2].toStdString();
        QStringList dates = upcomingApts[i][3].split("/");
        int day = dates[2].toInt();
        int month = dates[1].toInt();
        int year = dates[0].toInt();
        long int stdNum = upcomingApts[i][4].toLong();
        bool refill = bool(upcomingApts[i][5].toInt());
        bool pending = bool(upcomingApts[i][6].toInt());
        bool reminderB = bool(upcomingApts[i][7].toInt());
        appointment upcoming(id, day, month, year, stdNum, doctor, time, refill, pending, "", reminderB);
        clinicianUpcomingApts.append(upcoming);
        QPushButton *button = new QPushButton(aptText);
        signalMapperUpcoming->setMapping(button, i);
        connect(button,SIGNAL(clicked()),signalMapperUpcoming,SLOT(map()));
        layout->addWidget( button );
    }
    }

    /************clinician pending List***********/
    QList<QStringList> pendingApts = currentClinician.viewPendingAppointments();
    if(!(pendingApts.isEmpty())){
    ui->clinicianPendingAppointmentList->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    ui->clinicianPendingAppointmentList->setWidgetResizable( true );

    QWidget *widget2 = new QWidget();
    ui->clinicianPendingAppointmentList->setWidget( widget2 );

    QVBoxLayout *layout2 = new QVBoxLayout();
    widget2->setLayout( layout2 );
    clinicianPendingApts.clear();

    QSignalMapper *signalMapperPending = new QSignalMapper(this);
    connect(signalMapperPending, SIGNAL(mapped(int)), this, SLOT(pendingAppointmentButtonClick(int)));

    for(int i=0; i<pendingApts.length(); i++){
        //QStringList aptInfo = {aptNum, doctor, time, date, stdID, refill, pending, reminder};
        QString aptText = QString::number(i+1) + ". Student:" + pendingApts[i][4] + "\nDoctor: " + pendingApts[i][1] + "\nDate: " + pendingApts[i][3] + "\nTime:" + pendingApts[i][2];
        long int id = pendingApts[i][0].toLong();
        string doctor = pendingApts[i][1].toStdString();
        string time = pendingApts[i][2].toStdString();
        QStringList dates = pendingApts[i][3].split("/");
        int day = dates[2].toInt();
        int month = dates[1].toInt();
        int year = dates[0].toInt();
        long int stdNum = stol(pendingApts[i][4].toStdString());
        bool refill = bool(pendingApts[i][5].toInt());
        bool pending = bool(pendingApts[i][6].toInt());
        bool reminderB = bool(pendingApts[i][7].toInt());
        appointment pendingApt(id, day, month, year, stdNum, doctor, time, refill, pending, "", reminderB);
        clinicianPendingApts.append(pendingApt);
        QPushButton *button = new QPushButton(aptText, widget2);
        signalMapperPending->setMapping(button, i);
        connect(button,SIGNAL(clicked()),signalMapperPending,SLOT(map()));
        layout2->addWidget( button );
    }
    }


}

void MainWindow::upcomingAppointmentButtonClick(int button)
{
    if(ui->clinicianUpcomingAppointmentFrame->isHidden())
    {
        ui->clinicianBookAppointmentFrame->setHidden(true);
        ui->clinicianPendingAppointmentFrame->setHidden(true);
        ui->clinicianToolboxCollapse->setHidden(false);
        ui->clinicianUpcomingAppointmentFrame->setHidden(false);
    }
    appointment selected = clinicianUpcomingApts[button];
    QSqlQuery query;
    query.prepare("SELECT firstName, lastName FROM students WHERE studentID = :stdID");
    string studentNum = to_string(selected.getStudentNum()).c_str();
    query.bindValue(":stdID", studentNum.c_str());
    query.exec();
    query.first();
    QString firstName = query.value(0).toString();
    QString lastName = query.value(1).toString();
    int day = selected.getDay();
    int month = selected.getMonth();
    int year = selected.getYear();
    QString date = QString::number(year) + "/" + QString::number(month) + "/" + QString::number(day);
    ui->reminderDate->setText(date);
    ui->reminderFirstName->setText(firstName);
    ui->reminderLastName->setText(lastName);
    ui->reminderStudentNumber->setText(to_string(selected.getStudentNum()).c_str());
    ui->reminderTime->setText(selected.getTime().c_str());
    ui->reminderDoctor->setText(selected.getDoctor().c_str());
    if(selected.getRefill())
        ui->reminderRefill->setText("Yes");
    else
        ui->reminderRefill->setText("No");
}

void MainWindow::pendingAppointmentButtonClick(int button){
    if(ui->clinicianPendingAppointmentFrame->isHidden())
    {
        ui->clinicianBookAppointmentFrame->setHidden(true);
        ui->clinicianUpcomingAppointmentFrame->setHidden(true);
        ui->clinicianToolboxCollapse->setHidden(false);
        ui->clinicianPendingAppointmentFrame->setHidden(false);
    }
    appointment selected = clinicianPendingApts[button];
    QSqlQuery query;
    query.prepare("SELECT firstName, lastName FROM students WHERE studentID = :stdID");
    string studentNum = to_string(selected.getStudentNum()).c_str();
    query.bindValue(":stdID", studentNum.c_str());
    query.exec();
    query.first();
    QString firstName = query.value(0).toString();
    QString lastName = query.value(1).toString();
    int day = selected.getDay();
    int month = selected.getMonth();
    int year = selected.getYear();
    QString date = QString::number(year) + "/" + QString::number(month) + "/" + QString::number(day);
    ui->pendingDate->setText(date);
    ui->pendingFirstName->setText(firstName);
    ui->pendingLastName->setText(lastName);
    ui->pendingStudentNUmber->setText(to_string(selected.getStudentNum()).c_str());
    ui->pendingTime->setText(selected.getTime().c_str());
    ui->pendingDoctor->setText(selected.getDoctor().c_str());
    if(selected.getRefill())
        ui->pendingRefill->setText("Yes");
    else
        ui->pendingRefill->setText("No");
}

void MainWindow::on_removeStudentFromQueue_clicked(){
    try {
        patientQueue.deQueue();
        if (patientQueue.getSize() == 0) {
            ui->nextStudentInQueue->setText("No students waiting");
            ui->studentCurrentQueueLabel->setText("");
            ui->queueSizeBox->setText(QString::number(patientQueue.getSize()));
        }
        else {
            QString nextStudentText = QString::fromStdString(patientQueue.top().getFirstName() + patientQueue.top().getLastName());
            ui->nextStudentInQueue->setText(nextStudentText);
            ui->queueSizeBox->setText(QString::number(patientQueue.getSize()));
            string currentQueueText = ui->studentCurrentQueueLabel->text().toStdString();
            size_t firstLineIndex = currentQueueText.find("\n");
            string newText = currentQueueText.substr(firstLineIndex+1);
            ui->studentCurrentQueueLabel->setText(QString::fromStdString(newText));
        }
    }
    catch (queueException) {
        QMessageBox msgBox;
        msgBox.setText("There are no students waiting.");
        msgBox.exec();
    }

}

void MainWindow::on_ClinicianLogout_clicked()
{
    ui->mainStack->setCurrentIndex(0);
    ui->clinicianStack->setCurrentIndex(0);
    ui->clinicianUpcomingAppointmentFrame->setHidden(true);
    ui->clinicianPendingAppointmentFrame->setHidden(true);
    ui->clinicianBookAppointmentFrame->setHidden(true);
    ui->clinicianToolboxCollapse->setHidden(true);
    ui->loginConfirmation->setText("");
    ui->clinicianUsername->setText("");
    ui->clinicianEmail->setText("");

    // clear any widgets of upcoming appointments if present
    QWidget *wid = ui->studentAppointments->widget();
    if(wid)
        wid->deleteLater();
    QWidget *wid2 = ui->clinicianPendingAppointmentList->widget();
    if(wid2)
        wid2->deleteLater();
    QWidget *wid3 = ui->clinicianUpcomingAppointmentList->widget();
    if(wid3)
        wid3->deleteLater();
    ui->studentBookAppointment->setEnabled(true);
}

void MainWindow::on_clinicianUpdateWalkInCode_clicked() {
    QString code;
    code = ui->walkInCode->text();
    patientQueue.setQueueCode(code.toStdString());
}

void MainWindow::on_studentLogin_clicked()
{

    QString studentEmailDB;
    qint32 studentNumberDB;
    QString studentFirstNameDB;
    QString studentLastNameDB;
    studentFirstNameDB = ui->inputStudent_firstName->text();
    studentLastNameDB = ui->inputStudent_lastName->text();
    studentEmailDB = ui->inputStudentEmail->text();
    studentNumberDB = ui->inputStudentNumber->text().toInt();
    // Initialize logged in student
    currentStudent.setEmail(studentEmailDB.toStdString());
    currentStudent.setFirstName(studentFirstNameDB.toStdString());
    currentStudent.setLastName(studentLastNameDB.toStdString());
    currentStudent.setStudentNumber(studentNumberDB);
    ui->queueSizeBox->setText(QString::number(patientQueue.getSize()));
    ui->joinQueueButton->setEnabled(true);

    QSqlQuery queryStudentLogin;
    queryStudentLogin.prepare("SELECT * FROM students WHERE firstName = :studentFirstNameDB AND lastName = :studentLastNameDB AND studentID = :studentNumberDB AND email = :studentEmailDB");
    queryStudentLogin.bindValue(":studentFirstNameDB", studentFirstNameDB);
    queryStudentLogin.bindValue(":studentLastNameDB", studentLastNameDB);
    queryStudentLogin.bindValue(":studentNumberDB", studentNumberDB);
    queryStudentLogin.bindValue(":studentEmailDB", studentEmailDB);
    if (!queryStudentLogin.exec())
        qWarning() << "MainWindow::StudentLoginClicked - ERROR: " << queryStudentLogin.lastError().text();
    if(queryStudentLogin.next()) {
           ui->studentStack->setCurrentIndex(1);
        } else {
           ui->studentLoginStatus->setText("Sorry, invalid credentials");
        }

    QSqlQuery queryStudentAppts;
    queryStudentAppts.prepare("SELECT reminder FROM appts WHERE studentID = :studentNumberDB AND reminderB = '1'");
    queryStudentAppts.bindValue(":studentNumberDB", studentNumberDB);
    if (!queryStudentAppts.exec())
        qWarning() << "MainWindow::StudentLoginDatabasePopulate - ERROR: " << queryStudentAppts.lastError().text();
    if(queryStudentAppts.next()){
        ui->reminderLabel->setText("reminder:" + queryStudentAppts.value(0).toString());
        QMessageBox::information(this,"reminder","you have a reminder for the upcoming appointment\nyou can view the reminder in the appointment information");
    }else{
        ui->reminderLabel->setText("no reminder");
    }

    QList<QStringList> upcomingAppt = currentStudent.viewUpcomingAppointment();

    if(!(upcomingAppt.isEmpty())){
    QWidget *widget = new QWidget();
    ui->studentAppointments->setWidget(widget);

    QVBoxLayout *layout = new QVBoxLayout();
    widget->setLayout(layout);
    for(int i=0; i<upcomingAppt.length(); i++){
        //QStringList aptInfo = {aptNum, doctor, time, date, stdID, refill, pending, reminder};
        QString aptText = "Doctor: " + upcomingAppt[i][1] + "\nDate: " + upcomingAppt[i][3] + "\nTime:" + upcomingAppt[i][2];
        currentAppointment.setID(upcomingAppt[i][0].toLong());
        currentAppointment.setDoctor(upcomingAppt[i][1].toStdString());
        currentAppointment.setTime(upcomingAppt[i][2].toStdString());
        QStringList dates = upcomingAppt[i][3].split("/");
        currentAppointment.setDay(dates[2].toInt());
        currentAppointment.setMonth(dates[1].toInt());
        currentAppointment.setYear(dates[0].toInt());
        currentAppointment.setStudentNum(upcomingAppt[i][4].toLong());
        currentAppointment.setRefil(bool(upcomingAppt[i][5].toInt()));
        currentAppointment.setPending(bool(upcomingAppt[i][6].toInt()));
        currentAppointment.setReminder(bool(upcomingAppt[i][7].toInt()));
        QPushButton *button = new QPushButton(aptText);
        connect(button,SIGNAL(clicked()),this,SLOT(stdUpcomingAppointmentButtonClick()));
        layout->addWidget( button );
    }
    }

    queryStudentAppts.prepare("SELECT * FROM appts WHERE studentID = :studentNumberDB AND pending = '0'");
    queryStudentAppts.bindValue(":studentNumberDB", studentNumberDB);
    if (!queryStudentAppts.exec())
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << queryStudentAppts.lastError().text();
    if(queryStudentAppts.next()) {
        ui->confirmationLabel->setText("confirmed");
    }else{
        ui->confirmationLabel->setText("pending");
    }

    //Student appointment booking code here to not allow the student to make more than one appointment at a time
    QStringList upcomingApt; // 2D QStringList of all past appointments
    QSqlQuery query;
    QDate today = QDate::currentDate();
    string studNumber = to_string(currentStudent.getStudentNumber());
    string date = to_string(today.year()) + "-" + to_string(today.month()) + "-" + to_string(today.day());
    query.prepare("SELECT * FROM appts WHERE pending = true AND studentID = :number AND appDate >= :today");
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
    QList<QStringList> newAppt = currentStudent.viewUpcomingAppointment();
    if (upcomingApt.isEmpty() && newAppt.isEmpty()){
        ui->studentBookAppointment->setEnabled(true);
    }
    else{
        ui->studentBookAppointment->setEnabled(false);
    }
}

void MainWindow::stdUpcomingAppointmentButtonClick(){
    ui->doctorLabel->setText((currentAppointment.getDoctor().c_str()));
    string aptDate = to_string(currentAppointment.getYear()) + "/" + to_string(currentAppointment.getMonth()) + "/" + to_string(currentAppointment.getDay());
    ui->appDateLabel->setText(aptDate.c_str());
    ui->appTimeLabel->setText(currentAppointment.getTime().c_str());
    if(currentAppointment.getRefill())
        ui->refillLabel->setText("Yes");
    else
        ui->refillLabel->setText("No");
    ui->studentPostBookingFrame->setHidden(false);
}

void MainWindow::on_studentLogout_clicked()
{
    ui->mainStack->setCurrentIndex(0);
    ui->studentStack->setCurrentIndex(0);
    QString empty("");
    ui->viewHistoryLabel->setText(empty);

    // clear any widgets of upcoming appointments if present
    QWidget *wid = ui->studentAppointments->widget();
    if(wid)
        wid->deleteLater();
    QWidget *wid2 = ui->clinicianPendingAppointmentList->widget();
    if(wid2)
        wid2->deleteLater();
    QWidget *wid3 = ui->clinicianUpcomingAppointmentList->widget();
    if(wid3)
        wid3->deleteLater();
    ui->studentBookAppointment->setEnabled(true);
}

void MainWindow::on_studentBookAppointmentSubmit_clicked()
{
    ui->studentBookAppointmentFrame->setHidden(true);
    ui->studentAppointmentConfirmantionFrame->setHidden(false);
    ui->studentConfirmationLabel->setText("Thanks For Booking,\nYou will receive a notification once your appointment is confirmed!\n\nFeel Free to Fill Out Health Questionnaire\nBefore Your Appointment!");
    ui->studentQuestionnaireButtonFromQueue->setHidden(true);
    ui->studentNoThanksFromQueue->setHidden(true);
    ui->studentQuestionnaireButton->setHidden(false);
    ui->studentNoThanks->setHidden(false);

    //Student appointment booking code here
    int year = ui->viewApptDate->date().year();
    int month = ui->viewApptDate->date().month();
    int day = ui->viewApptDate->date().day();

    int hour = ui->viewTimeAppt->time().hour();
    int minute = ui->viewTimeAppt->time().minute();
    string stringHour = to_string(hour);
    string stringMinute = to_string(minute);
    string stringSecond("00");

    if (stringHour.length()==1){
        stringHour = "0"+stringHour;
    }

    if (stringMinute.length()==1){
        stringMinute = "0"+stringMinute;
    }

    QString newDoctor = ui->viewDoctor->currentText();
    bool refill(false);
    if (ui->refillNo->isChecked()){
        refill = false;
    }
    if (ui->refillYes->isChecked()) {
        refill = true;
    }

    //query to get appt ID
    QSqlQuery query;
    string date = to_string(year) + "/" + to_string(month) + "/" + to_string(day);
    string time = stringHour + ":" + stringMinute + ":" + stringSecond;
    string doctor = newDoctor.toStdString();
    query.prepare("SELECT apptID FROM appts WHERE appDate = :date AND apptTime = :time AND doctor = :doctor");
    query.bindValue(":date", date.c_str());
     query.bindValue(":time", time.c_str());
    query.bindValue(":doctor", doctor.c_str());
    if(!query.exec()){
        qWarning() << "MainWindow::DatabaseDelete - ERROR: " << query.lastError().text();
    }
   //find appt number where this exists
    query.first();
    QString aptNum = query.value(0).toString();
    long int appointmentNumber = aptNum.toInt();
    string reminderText("");
    appointment NewAppointment(appointmentNumber,day,month,year,currentStudent.getStudentNumber(),doctor,time,refill,true,reminderText,false);
    NewAppointment.pendingApointment();

    QList<QStringList> upcomingAppt = currentStudent.viewUpcomingAppointment();

    if(!(upcomingAppt.isEmpty())){
    QWidget *widget = new QWidget();
    ui->studentAppointments->setWidget(widget);

    QVBoxLayout *layout = new QVBoxLayout();
    widget->setLayout(layout);
    cout<<upcomingAppt.length()<<endl;
    for(int i=0; i<upcomingAppt.length(); i++){
        //QStringList aptInfo = {aptNum, doctor, time, date, stdID, refill, pending, reminder};
        QString aptText = "Doctor: " + upcomingAppt[i][1] + "\nDate: " + upcomingAppt[i][3] + "\nTime:" + upcomingAppt[i][2];
        currentAppointment.setID(upcomingAppt[i][0].toLong());
        currentAppointment.setDoctor(upcomingAppt[i][1].toStdString());
        currentAppointment.setTime(upcomingAppt[i][2].toStdString());
        QStringList dates = upcomingAppt[i][3].split("/");
        currentAppointment.setDay(dates[2].toInt());
        currentAppointment.setMonth(dates[1].toInt());
        currentAppointment.setYear(dates[0].toInt());
        currentAppointment.setStudentNum(upcomingAppt[i][4].toLong());
        currentAppointment.setRefil(bool(upcomingAppt[i][5].toInt()));
        currentAppointment.setPending(bool(upcomingAppt[i][6].toInt()));
        currentAppointment.setReminder(bool(upcomingAppt[i][7].toInt()));
        QPushButton *button = new QPushButton(aptText);
        connect(button,SIGNAL(clicked()),this,SLOT(stdUpcomingAppointmentButtonClick()));
        layout->addWidget( button );
    }
    }

    ui->studentBookAppointment->setEnabled(false);
}

void MainWindow::on_studentBookAppointment_clicked()
{
    /******GUI Code****/
    ui->studentAppointmentConfirmantionFrame->setHidden(true);
    ui->studentPostBookingFrame->setHidden(true);
    ui->studentBookAppointmentFrame->setHidden(false);
    ui->studentToolboxCollapse->setHidden(false);
    /******GUI Code****/


}

void MainWindow::on_studentToolboxCollapse_clicked()
{
    ui->studentAppointmentConfirmantionFrame->setHidden(true);
    ui->studentBookAppointmentFrame->setHidden(true);
    ui->studentPostBookingFrame->setHidden(true);
    ui->studentToolboxCollapse->setHidden(true);

}

void MainWindow::on_joinQueueButton_clicked()
{
    ui->studentAppointmentConfirmantionFrame->setHidden(true);
    ui->studentPostBookingFrame->setHidden(true);
    if(ui->enterQueueCode->text() != QString::fromStdString(patientQueue.getQueueCode()))//Your Field checking, I am only checking is it empty or not.
    {
        QMessageBox msgBox;
        msgBox.setText("The code you entered is not correct.");
        msgBox.exec();
    }
    else{//if queue code is correct

        patientQueue.enQueue(currentStudent);
        ui->queueSizeBox->setText(QString::number(patientQueue.getSize()));
        QString temp = ui->studentCurrentQueueLabel->text();
        temp.append(QString::fromStdString(currentStudent.getFirstName()+" "+currentStudent.getLastName()+"\n"));
        ui->studentCurrentQueueLabel->setText(temp);
        ui->studentConfirmationLabel->setText("You've been added to the queue\nPlease see your position to the left.\n\nFeel Free to Fill Out Health Questionnaire\nBefore Your Appointment!");
        ui->studentBookAppointmentFrame->setHidden(true);
        ui->studentAppointmentConfirmantionFrame->setHidden(false);
        ui->studentToolboxCollapse->setHidden(false);
        ui->studentQuestionnaireButtonFromQueue->setHidden(false);
        ui->studentNoThanksFromQueue->setHidden(false);
        ui->studentQuestionnaireButton->setHidden(true);
        ui->studentNoThanks->setHidden(true);
        ui->joinQueueButton->setEnabled(false);
    }

}

void MainWindow::on_studentCancelAppointment_clicked()
{
    if(QMessageBox::question(this,"Cancel Appointment","Are You Sure?\nYou Want To Cancel This Appointment.") == QMessageBox::Yes)
    {
        ui->studentToolboxCollapse->clicked(true);
        //code for appointment cancellation goes here
        currentStudent.cancelAppointment(currentAppointment);
        // clear any widgets of upcoming appointments if present
        QWidget *wid = ui->studentAppointments->widget();
        if(wid)
            wid->deleteLater();
        QWidget *wid2 = ui->clinicianPendingAppointmentList->widget();
        if(wid2)
            wid2->deleteLater();
        QWidget *wid3 = ui->clinicianUpcomingAppointmentList->widget();
        if(wid3)
            wid3->deleteLater();
        ui->studentBookAppointment->setEnabled(true);

    }
    else{
        //Do nothing
    }
}

void MainWindow::on_studentNoThanks_clicked()
{
    ui->studentAppointmentConfirmantionFrame->setHidden(true);
    ui->studentPostBookingFrame->setHidden(false);
    ui->studentOptionalQuestionnaireButton->setHidden(false);
}


void MainWindow::on_studentQuestionnaireButton_clicked()
{
    ui->studentStack->setCurrentIndex(3);
    ui->studentAppointmentConfirmantionFrame->setHidden(true);
    ui->studentPostBookingFrame->setHidden(false);
    ui->studentOptionalQuestionnaireButton->setHidden(true);
}

void MainWindow::on_studentQuestionnaireSubmit_clicked()
{
    QString studentAllergies; // Setting up questionnaire variables
    QString studentMedication;
    qint32 studentIDQ;
    bool studentSmoker = NULL;
    bool studentPastPatient = NULL;

    qint8 studentPain;

    studentAllergies = ui->questionThreeAnswer->text(); // Obtaining variable values from ui
    studentMedication = ui->questionFourAnswer->text();
    studentIDQ =currentStudent.getStudentNumber(); // Obtaining student number from current student instance

    if (ui->smokingYes->isChecked()) {
        studentSmoker = true;
    }
    else if (ui->smokingNo->isChecked()) {
        studentSmoker = false;
    }

    if (ui->questionFiveYes->isChecked()) {
        studentPastPatient = true;
    }
    else if (ui->questionFiveNo->isChecked()) {
        studentPastPatient = false;
    }
    QSqlQuery queryQuestionnaire; // Query connecting to questionnaire database and filling info filled by student
    queryQuestionnaire.prepare("INSERT INTO questionnaire(studentID, allergies, medication, smoker, pastPatient) VALUES(:studentIDQ, :studentAllergies, :studentMedication, :studentSmoker, :studentPastPatient)");
    queryQuestionnaire.bindValue(":studentIDQ", studentIDQ);
    queryQuestionnaire.bindValue(":studentAllergies", studentAllergies);
    queryQuestionnaire.bindValue(":studentMedication", studentMedication);
    queryQuestionnaire.bindValue(":studentSmoker", studentSmoker);
    queryQuestionnaire.bindValue(":studentPastPatient", studentPastPatient);

    if(!queryQuestionnaire.exec())
        qWarning() << "MainWindow::DatabaseDelete - ERROR: " << queryQuestionnaire.lastError().text();

    ui->studentStack->setCurrentIndex(1);
}

void MainWindow::on_studentOptionalQuestionnaireButton_clicked()
{
    ui->studentOptionalQuestionnaireButton->setHidden(true);
    ui->studentStack->setCurrentIndex(3);
}

void MainWindow::on_studentPostBookingFinish_clicked()
{
    ui->studentToolboxCollapse->clicked(true);
    ui->studentOptionalQuestionnaireButton->setHidden(false);
    ui->studentStack->setCurrentIndex(1);
}

void MainWindow::on_studentQuestionnaireButtonFromQueue_clicked()
{

    ui->studentToolboxCollapse->clicked(true);
    ui->studentStack->setCurrentIndex(3);
}

void MainWindow::on_studentNoThanksFromQueue_clicked()
{
    ui->studentToolboxCollapse->clicked(true);
}

void MainWindow::clock()
{
    QTime *timer = new QTime();
    timer->start();

    QDate *date = new QDate();
    QLocale *local = new QLocale;
    QString dat = local->monthName(date->currentDate().month()) + " " + QString("%1, %2").arg(date->currentDate().day()).arg(date->currentDate().year());

    ui->Time_Date_clinician->setText(dat);
    ui->Time_Date_student->setText(dat);


    if(timer->minute() > 9 && timer->hour() > 9)
    {
        QString tim = QString("%1:%2").arg(timer->hour()).arg(timer->minute());
        ui->Time_Time_clinician->setText(tim);
        ui->Time_Time_student->setText(tim);

    }
    else if(timer->minute() < 10 && timer->hour() > 9){

        QString tim = QString("%1:0%2").arg(timer->hour()).arg(timer->minute());
        ui->Time_Time_clinician->setText(tim);
        ui->Time_Time_student->setText(tim);

    }
    else if (timer->minute() < 10 && timer->hour() < 10) {
        QString tim = QString("0%1:0%2").arg(timer->hour()).arg(timer->minute());
        ui->Time_Time_clinician->setText(tim);
        ui->Time_Time_student->setText(tim);
    }
    else if (timer->minute() > 9 && timer->hour() < 10) {
        QString tim = QString("0%1:%2").arg(timer->hour()).arg(timer->minute());
        ui->Time_Time_clinician->setText(tim);
        ui->Time_Time_student->setText(tim);
    }

}

void MainWindow::on_viewApptHistory_clicked()
{
    ui->studentStack->setCurrentIndex(2);
    QList<QStringList> pastAppt = currentStudent.viewHistory();
    QString temp = ui->viewHistoryLabel->text();
    for(int i=0; i<pastAppt.length(); i++){
        QString refill;
        if (pastAppt[i][4]=="1"){
            refill="Yes";
        }
        else {
            refill="No";
        }
        //QStringList aptInfo = {doctor, time, date, stdID, refill, pending, reminder};
        QString aptText = "Doctor: " + pastAppt[i][0] + ", Date: " + pastAppt[i][2] + ", Time: " + pastAppt[i][1]+", Refilled a prescription: "+refill;
        temp.append(aptText+"\n");
    }
    ui->viewHistoryLabel->setText(temp);
}

void MainWindow::on_buttonHome_clicked()
{
     ui->studentStack->setCurrentIndex(1);
     QString empty("");
     ui->viewHistoryLabel->setText(empty);
}


void MainWindow::on_clinicianCheckQuestionnaire_clicked()
{
    qint32 studentIDQuestionnaire;
    studentIDQuestionnaire = ui->clinicianStudentIDQ->text().toInt();

    QSqlQuery queryStudentQuestionnare;
    queryStudentQuestionnare.prepare("SELECT * FROM questionnaire WHERE studentID = :studentIDQuestionnaire LIMIT 1"); // Only one questionnaire (latest)

    queryStudentQuestionnare.bindValue(":studentIDQuestionnaire", studentIDQuestionnaire);
    if (!queryStudentQuestionnare.exec())
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << queryStudentQuestionnare.lastError().text();
    if(queryStudentQuestionnare.next()) {
        if(queryStudentQuestionnare.value(3) == true) {
            ui->smokerQ->setText("Yes");
        } else if(queryStudentQuestionnare.value(3) == false) {
            ui->smokerQ->setText("No");
        }
        if(queryStudentQuestionnare.value(4) == true) {
            ui->pastPatientQ->setText("Yes");
        } else if(queryStudentQuestionnare.value(4) == false) {
            ui->pastPatientQ->setText("No");
        }
        ui->allergiesQ->setText(queryStudentQuestionnare.value(1).toString());
        ui->medicationQ->setText(queryStudentQuestionnare.value(2).toString());
        } else {
            //ui->smokerQ->setText("DIDNT WORK");
        return;

        }
}
