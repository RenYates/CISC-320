#include "queue.h"
#include <iostream>
#include "student.h"
using namespace std;

// queueException implementation, message must be supplied when thrown
queueException::queueException(const string &message) : message(message) {}
string& queueException::what() { return message; }

queue::queue(){
    size = 0;
    front = -1;
    rear = -1;
    queueCode = "0000";
}

string queue::getQueueCode() {
    return queueCode;
}

void queue::setQueueCode(string code) {
    queueCode = code;
}


bool queue::isFull(){
    if (front == 0 && rear == MAX_SIZE-1) {
        return true;
    }
    else {
        return false;
    }
}
bool queue::isEmpty(){
    if (front == -1) {
        return true;
    }
    else {
        return false;
    }
}

int queue::getSize(){
    return size;
}

student queue::top(){
    student element;
    element = patientQueue[front];
    return element;
}

void queue::enQueue(student element){
    if (this->isFull()) {
        throw queueException("The queue is full, cannot add another student.");
    }
    else {
        if (front == -1) front = 0;
        rear++;
        size++;
        patientQueue[rear] = element;
    }
}

student queue::deQueue(){
    student element;
    if (this->isEmpty()) {
        throw queueException("The queue is empty, cannot remove a student.");
    }
    else {
        element = patientQueue[front];
        size--;
        if (front >= rear) {
            front = -1;
            rear = -1;
        }
        else {
            front++;
        }
        return element;
    }
}
