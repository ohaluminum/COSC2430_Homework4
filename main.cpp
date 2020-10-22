#include <iostream>
#include <fstream>
#include <sstream>
#include "ArgumentManager.h"
using namespace std;

struct process
{
    int processNumber;
    int executionTime;
    int waitTime;
    int startTime;
    int endTime;
    int remaining;

    process* next;
};

class queue
{
private:
    process* front;
    process* back;
    int size;

public:

    queue()
    {
        front = nullptr;
        back = nullptr;
        size = 0;
    }

    bool isEmpty()
    {
        if (size == 0)
        {
            return true;
        }

        return false;
    }

    process* getFront()
    {
        return front;
    }

    process* getBack()
    {
        return back;
    }

    int getSize()
    {
        return size;
    }

    //Add the new precess at the end of the queue
    void push(int number, int execution, int wait, int start, int end, int remaining)
    {
        //1.Create a temp process
        process* temp = new process;

        //2.Fill the data
        temp->processNumber = number;
        temp->executionTime = execution;
        temp->waitTime = wait;
        temp->startTime = start;
        temp->endTime = end;
        temp->remaining = remaining;

        temp->next = nullptr;

        //3.Update the pointer
        if (isEmpty())
        {
            front = temp;
            back = temp;
        }

        else if (size > 0)
        {
            back->next = temp;
            back = temp;
        }

        //4.Update the size
        size++;
    }

    //Delete the process at the beginning of the queue 
    void pop()
    {
        //1.Create a temp precoss
        process* temp = new process;
        temp = front;

        //2.Update the pointer
        if (isEmpty())
        {
            return;
        }

        else if (size == 1)
        {
            front = nullptr;
            back = nullptr;
        }

        else if (size > 1)
        {
            front = front->next;
        }

        //3.Delete the first process
        delete temp;

        //4.Update the size
        size--;
    }
};

int main(int argc, char* argv[])
{
    ArgumentManager am(argc, argv);

    //Get the filename of argument name "input" and "output" and "command"
    //string input = am.get("input");
    //string output = am.get("output");
    //string command = am.get("command");

    //Test
    string input = "input41.txt";
    string output = "output41.txt";

    ifstream inFS;
    ofstream outFS;
    istringstream inSS;

    try
    {
        //Open input file and output file  
        inFS.open(input);
        outFS.open(output);

        //Throw exception if the file doesn't exist
        if (!inFS.is_open())
        {
            throw runtime_error("ERROR: File not found");
        }

        //Throw exception if the file is empty
        if (inFS.peek() == EOF)
        {
            throw runtime_error("ERROR: File is empty");
        }

        int maxTime = 0;
        int numOfProcess = 0;
        int numOfServer = 0;
        int executionTime = 0;

        queue ProcessQueue;
        queue completedProcess;

        int timer = 0;
        process* temp;

        //Read input file 
        inFS >> maxTime >> numOfProcess >> numOfServer;

        for (int i = 0; i < numOfProcess; i++)
        {
            inFS >> executionTime;
            ProcessQueue.push(i + 1, executionTime, 0, 0, 0, executionTime);
        }

        //Create a dynamic array as server station 
        process** server = new process*[numOfServer];

        //Initiate the dynamic array with nullptr
        for (int i = 0; i < numOfServer; i++)
        {
            server[i] = nullptr;
        }

        //If the queue is not empty (has process left)
        while (completedProcess.getSize() != numOfProcess)
        {


            //Fill the available server: These tasks are in-process!
            for (int i = 0; i < numOfServer; i++)
            {
                if (server[i] == nullptr && !ProcessQueue.isEmpty())
                {
                    cout << "yes" << endl;
                    server[i] = new process;
                    server[i]->processNumber = ProcessQueue.getFront()->processNumber;
                    server[i]->executionTime = ProcessQueue.getFront()->executionTime;
                    server[i]->waitTime = ProcessQueue.getFront()->waitTime;
                    server[i]->startTime = ProcessQueue.getFront()->startTime;
                    server[i]->endTime = ProcessQueue.getFront()->endTime;
                    server[i]->remaining = ProcessQueue.getFront()->remaining;

                    ProcessQueue.pop();

                    server[i]->startTime = timer;

                    if (server[i]->remaining == 0)
                    {
                        server[i]->endTime = timer;
                    }                                                                         //DELETE

                    cout << i << " " << server[i]->processNumber << server[i]->executionTime << server[i]->waitTime << server[i]->startTime << server[i]->endTime << server[i]->remaining << endl;
                }
            }

            //One minute passed
            timer++;

            //Add waiting time for all waiting tasks
            temp = ProcessQueue.getFront();

            while (temp != nullptr)
            {
                temp->waitTime = temp->waitTime + 1;

                cout << temp->waitTime << endl;                              //DELETE!!!

                temp = temp->next;
            }

            //Check if tasks are completed
            for (int i = 0; i < numOfServer; i++)
            {
                if (server[i] != nullptr)
                {
                    server[i]->remaining = server[i]->remaining - 1;

                    cout << i << " " << server[i]->processNumber << server[i]->executionTime << server[i]->waitTime << server[i]->startTime << server[i]->endTime << server[i]->remaining << endl;
                                                                           //DELETE
                    if (server[i]->remaining == 0)
                    {
                        server[i]->endTime = timer;
                        completedProcess.push(server[i]->processNumber, server[i]->executionTime, server[i]->waitTime, server[i]->startTime, server[i]->endTime, 0);

                        //Clear server for the next task
                        server[i] = nullptr;
                    }
                }
            }
        }


        process* head = completedProcess.getFront();
        while (head != nullptr)
        {
            cout << head->processNumber << head->executionTime << head->waitTime << head->startTime << head->endTime << endl;
            head = head->next;
        }

        
        
    }
    catch (runtime_error & e)
    {
        outFS << e.what() << endl;
    }

    //Close files
    inFS.close();
    outFS.close();

    return 0;
}
