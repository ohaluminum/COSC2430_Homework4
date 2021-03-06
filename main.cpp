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

    int remainingTime;
    int currentTime;

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

    void print(int numOfProcess, ofstream& outFS)
    {
        //1.Create a temp process
        process* temp = new process;

        for (int i = 1; i <= numOfProcess; i++)
        {
            temp = front;

            while (temp != nullptr)
            {
                //If find the position of target number
                if (temp->processNumber == i)
                {
                    outFS << "Process: " << temp->processNumber << endl;
                    outFS << "Execution time: " << temp->executionTime << endl;
                    outFS << "Wait Time: " << temp->waitTime << endl;
                    outFS << "Start Time: " << temp->startTime << endl;

                    //If it is the last process
                    if (i == numOfProcess)
                    {
                        outFS << "End Time: " << temp->endTime;
                    }
                    else
                    {
                        outFS << "End Time: " << temp->endTime << endl << endl;
                    }

                    break;
                }

                temp = temp->next;
            }
        }
    }

    //Add the new precess at the end of the queue
    void push(int number, int execution, int wait, int start, int end, int remaining, int currentTime)
    {
        //1.Create a temp process
        process* temp = new process;

        //2.Fill the data
        temp->processNumber = number;
        temp->executionTime = execution;
        temp->waitTime = wait;
        temp->startTime = start;
        temp->endTime = end;
        temp->remainingTime = remaining;
        temp->currentTime = currentTime;

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
    string input = am.get("input");
    string output = am.get("output");

    //Test
    //string input = "input47.txt";
    //string output = "output47.txt";

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

        int timer = 0;
        process* temp;

        //Read input file 
        inFS >> maxTime >> numOfProcess >> numOfServer;

        //Write output file
        outFS << "Max time: " << maxTime << endl;
        outFS << "Number of processes: " << numOfProcess << endl;
        outFS << "Number of servers: " << numOfServer << endl << endl;

        //Create a queue as process queue
        queue ProcessQueue;

        for (int i = 0; i < numOfProcess; i++)
        {
            inFS >> executionTime;
            ProcessQueue.push(i + 1, executionTime, 0, 0, 0, executionTime, 0);
        }

        //Create a queue as completed queue
        queue completedProcess;

        //Create a dynamic array as server station 
        process** server = new process*[numOfServer];

        //Initiate the dynamic array with nullptr
        for (int i = 0; i < numOfServer; i++)
        {
            server[i] = nullptr;
        }

        //If the queue is not full (has process left)
        while (completedProcess.getSize() < numOfProcess)
        {
            //Fill the available server: These tasks are in-process!
            for (int i = 0; i < numOfServer; i++)
            {
                //If the server is empty and the next process execution time is 0, it should be completed immediately
                while (server[i] == nullptr && !ProcessQueue.isEmpty() && ProcessQueue.getFront()->executionTime == 0)
                {
                    ProcessQueue.getFront()->startTime = timer;
                    ProcessQueue.getFront()->endTime = timer;

                    completedProcess.push(ProcessQueue.getFront()->processNumber, ProcessQueue.getFront()->executionTime, ProcessQueue.getFront()->waitTime, ProcessQueue.getFront()->startTime, ProcessQueue.getFront()->endTime, 0, 0);

                    ProcessQueue.pop();
                }

                //If the server is empty and the process queue has process left
                if (server[i] == nullptr && !ProcessQueue.isEmpty())
                {
                    server[i] = new process;
                    server[i]->processNumber = ProcessQueue.getFront()->processNumber;
                    server[i]->executionTime = ProcessQueue.getFront()->executionTime;
                    server[i]->waitTime = ProcessQueue.getFront()->waitTime;
                    server[i]->startTime = ProcessQueue.getFront()->startTime;
                    server[i]->endTime = ProcessQueue.getFront()->endTime;
                    server[i]->remainingTime = ProcessQueue.getFront()->remainingTime;
                    server[i]->currentTime = ProcessQueue.getFront()->currentTime;

                    ProcessQueue.pop();

                    //If the process is a new process (not re-start process)
                    if (server[i]->executionTime == server[i]->remainingTime)
                    {
                        server[i]->startTime = timer;
                    }
                }

            }

            //One minute passed
            timer++;

            //Add waiting time for all waiting tasks
            temp = ProcessQueue.getFront();

            while (temp != nullptr)
            {
                temp->waitTime = temp->waitTime + 1;

                temp = temp->next;
            }

            //Check if tasks are completed
            for (int i = 0; i < numOfServer; i++)
            {
                //If the server is working on process
                if (server[i] != nullptr)
                {
                    server[i]->remainingTime = server[i]->remainingTime - 1;
                    server[i]->currentTime = server[i]->currentTime + 1;

                    if (server[i]->remainingTime == 0)
                    {
                        server[i]->endTime = timer;

                        completedProcess.push(server[i]->processNumber, server[i]->executionTime, server[i]->waitTime, server[i]->startTime, server[i]->endTime, 0, 0);

                        //Clear server for the next task
                        server[i] = nullptr;
                    }

                    else if (server[i]->currentTime == maxTime)
                    {
                        ProcessQueue.push(server[i]->processNumber, server[i]->executionTime, server[i]->waitTime, server[i]->startTime, server[i]->endTime, server[i]->remainingTime, 0);

                        //Clear server for the next task
                        server[i] = nullptr;
                    }
                }
            }
        }

        //Write output file 
        completedProcess.print(numOfProcess, outFS);

        

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
