CREATE TABLE students (
	firstName VARCHAR(20) NOT NULL,
	lastName VARCHAR(20) NOT NULL,
	studentID bigint PRIMARY KEY NOT NULL UNIQUE,
	email VARCHAR(30)NOT NULL UNIQUE
);
CREATE TABLE clinician (
	username VARCHAR(30) PRIMARY KEY NOT NULL UNIQUE,
	email VARCHAR(30)NOT NULL UNIQUE
);
CREATE TABLE appts (
	apptID bigint PRIMARY KEY NOT NULL UNIQUE,
	doctor VARCHAR(30) DEFAULT NULL,
	apptTime time,
	appDate date,
	studentID bigint NULL,
	refill bool,
	pending bool DEFAULT true,
	reminder VARCHAR(100) DEFAULT NULL,
	reminderB bool DEFAULT false,
	FOREIGN KEY (studentID) REFERENCES students(studentID)
);
CREATE TABLE questionnaire (
	studentID bigint PRIMARY KEY NULL,
	allergies VARCHAR(50) NULL,
	medication VARCHAR(50) NULL,
    smoker bool NULL,
    pastPatient bool NULL,
    FOREIGN KEY (studentID) REFERENCES students(studentID)
);
