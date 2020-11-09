#ifndef QUEUE_H
#define QUEUE_H
#include "student.h"
// We can adjust this, not sure what we want to go with
// Should be equivalent to max people in clinic
#define MAX_SIZE 15


class queue
{
private:
    student patientQueue[MAX_SIZE];
    int front;
    int rear;
    int size;
    string queueCode;
public:
    queue();
    bool isFull();
    bool isEmpty();
    int getSize();
    void enQueue(student element);
    student top();
    student deQueue();
    string getQueueCode();
    void setQueueCode(string code);
};

class queueException {
public:
    queueException(const string& message);
    string& what();
private:
    string message; // Message to be specified when exception is thrown
};


#endif // QUEUE_H
