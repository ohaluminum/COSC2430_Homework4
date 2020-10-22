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

    //Add the new precess at the end of the queue
    void push(int number, int execution, int wait, int start, int end)
    {
        //1.Create a temp process
        process* temp = new process;

        //2.Fill the data
        temp->processNumber = number;
        temp->executionTime = execution;
        temp->waitTime = wait;
        temp->startTime = start;
        temp->endTime = end;

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

        //Read input file 
        inFS >> maxTime >> numOfProcess >> numOfServer;

        for (int i = 0; i < numOfProcess; i++)
        {
            inFS >> executionTime;
            ProcessQueue.push(i + 1, executionTime, 0, 0, 0);
        }

        process* head = ProcessQueue.getFront();
        while (head != nullptr)
        {
            cout << head->processNumber << head->executionTime << endl;
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
