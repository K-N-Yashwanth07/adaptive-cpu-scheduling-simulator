#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>

using namespace std;

struct Process
{
    int pid;
    int arrival;
    int burst;
    int priority;

    int completion = 0;
    int turnaround = 0;
    int waiting = 0;
    int remaining = 0;
};

void calculateTimes(vector<Process> &p)
{
    for(auto &pr : p)
    {
        pr.turnaround = pr.completion - pr.arrival;
        pr.waiting = pr.turnaround - pr.burst;
    }
}

void printTable(vector<Process> p)
{
    float totalWT = 0, totalTAT = 0;

    cout << "\nPID\tAT\tBT\tPR\tCT\tTAT\tWT\n";

    for(auto &pr : p)
    {
        cout << pr.pid << "\t"
             << pr.arrival << "\t"
             << pr.burst << "\t"
             << pr.priority << "\t"
             << pr.completion << "\t"
             << pr.turnaround << "\t"
             << pr.waiting << "\n";

        totalWT += pr.waiting;
        totalTAT += pr.turnaround;
    }

    cout << "\nAverage Waiting Time: " << totalWT / p.size();
    cout << "\nAverage Turnaround Time: " << totalTAT / p.size() << endl;
}

void printGantt(vector<int> order)
{
    cout << "\nGantt Chart:\n|";
    for(int id : order)
        cout << " P" << id << " |";
    cout << "\n";
}

void sortArrival(vector<Process> &p)
{
    sort(p.begin(), p.end(), [](Process a, Process b){
        return a.arrival < b.arrival;
    });
}

float fcfs(vector<Process> p)
{
    sortArrival(p);

    int time = 0;
    vector<int> order;

    for(auto &pr : p)
    {
        if(time < pr.arrival)
            time = pr.arrival;

        order.push_back(pr.pid);

        time += pr.burst;
        pr.completion = time;
    }

    calculateTimes(p);

    printGantt(order);
    printTable(p);

    float avg = 0;
    for(auto &pr : p)
        avg += pr.waiting;

    return avg / p.size();
}

float sjf(vector<Process> p)
{
    int n = p.size();
    vector<bool> done(n,false);
    int completed = 0, time = 0;

    vector<int> order;

    while(completed < n)
    {
        int idx = -1;
        int minBurst = 1e9;

        for(int i=0;i<n;i++)
        {
            if(!done[i] && p[i].arrival <= time && p[i].burst < minBurst)
            {
                minBurst = p[i].burst;
                idx = i;
            }
        }

        if(idx == -1)
        {
            time++;
            continue;
        }

        order.push_back(p[idx].pid);

        time += p[idx].burst;
        p[idx].completion = time;

        done[idx] = true;
        completed++;
    }

    calculateTimes(p);

    printGantt(order);
    printTable(p);

    float avg = 0;
    for(auto &pr : p)
        avg += pr.waiting;

    return avg / p.size();
}

float priorityScheduling(vector<Process> p)
{
    int n = p.size();

    vector<bool> done(n,false);
    int completed = 0, time = 0;

    vector<int> order;

    while(completed < n)
    {
        int idx = -1;
        int best = 1e9;

        for(int i=0;i<n;i++)
        {
            if(!done[i] && p[i].arrival <= time && p[i].priority < best)
            {
                best = p[i].priority;
                idx = i;
            }
        }

        if(idx == -1)
        {
            time++;
            continue;
        }

        order.push_back(p[idx].pid);

        time += p[idx].burst;
        p[idx].completion = time;

        done[idx] = true;
        completed++;
    }

    calculateTimes(p);

    printGantt(order);
    printTable(p);

    float avg = 0;
    for(auto &pr : p)
        avg += pr.waiting;

    return avg / p.size();
}

float roundRobin(vector<Process> p, int quantum)
{
    int n = p.size();

    queue<int> q;
    vector<int> order;

    for(int i=0;i<n;i++)
        p[i].remaining = p[i].burst;

    int time = 0;

    q.push(0);

    while(!q.empty())
    {
        int i = q.front();
        q.pop();

        int exec = min(quantum, p[i].remaining);

        order.push_back(p[i].pid);

        time += exec;
        p[i].remaining -= exec;

        for(int j=0;j<n;j++)
        {
            if(p[j].arrival <= time && p[j].remaining > 0 && j != i)
            {
                bool exists = false;
                queue<int> temp = q;

                while(!temp.empty())
                {
                    if(temp.front() == j)
                        exists = true;
                    temp.pop();
                }

                if(!exists)
                    q.push(j);
            }
        }

        if(p[i].remaining > 0)
            q.push(i);
        else
            p[i].completion = time;
    }

    calculateTimes(p);

    printGantt(order);
    printTable(p);

    float avg = 0;
    for(auto &pr : p)
        avg += pr.waiting;

    return avg / p.size();
}

float srtf(vector<Process> p)
{
    int n = p.size();
    int time = 0, completed = 0;

    vector<int> order;

    for(int i=0;i<n;i++)
        p[i].remaining = p[i].burst;

    while(completed < n)
    {
        int idx = -1;
        int minRem = 1e9;

        for(int i=0;i<n;i++)
        {
            if(p[i].arrival <= time && p[i].remaining > 0 && p[i].remaining < minRem)
            {
                minRem = p[i].remaining;
                idx = i;
            }
        }

        if(idx == -1)
        {
            time++;
            continue;
        }

        order.push_back(p[idx].pid);

        p[idx].remaining--;
        time++;

        if(p[idx].remaining == 0)
        {
            p[idx].completion = time;
            completed++;
        }
    }

    calculateTimes(p);

    printGantt(order);
    printTable(p);

    float avg = 0;
    for(auto &pr : p)
        avg += pr.waiting;

    return avg / p.size();
}

void multilevelQueue(vector<Process> p)
{
    vector<Process> systemQ;
    vector<Process> interactiveQ;
    vector<Process> batchQ;

    for(auto &pr : p)
    {
        if(pr.priority == 1)
            systemQ.push_back(pr);
        else if(pr.priority == 2)
            interactiveQ.push_back(pr);
        else
            batchQ.push_back(pr);
    }

    cout << "\nSystem Queue (Priority Scheduling)\n";
    if(!systemQ.empty())
        priorityScheduling(systemQ);

    cout << "\nInteractive Queue (Round Robin)\n";
    if(!interactiveQ.empty())
        roundRobin(interactiveQ,2);

    cout << "\nBatch Queue (FCFS)\n";
    if(!batchQ.empty())
        fcfs(batchQ);
}

int adaptiveChoice(vector<Process> p)
{
    int n = p.size();

    float avgBurst = 0;

    for(auto &pr : p)
        avgBurst += pr.burst;

    avgBurst /= n;

    int smallJobs = 0;

    for(auto &pr : p)
        if(pr.burst < avgBurst)
            smallJobs++;

    if(n > 8)
        return 4;

    if(smallJobs > n/2)
        return 2;

    for(auto &pr : p)
        if(pr.priority == 1)
            return 3;

    return 1;
}

void compareAlgorithms(vector<Process> p)
{
    cout << "\nAlgorithm Comparison\n";

    cout << "\nFCFS\n";
    fcfs(p);

    cout << "\nSJF\n";
    sjf(p);

    cout << "\nPriority\n";
    priorityScheduling(p);

    cout << "\nRound Robin\n";
    roundRobin(p,2);

    cout << "\nSRTF\n";
    srtf(p);
}

void readFromFile(vector<Process> &p)
{
    ifstream file("input.txt");

    int n;
    file >> n;

    p.resize(n);

    for(int i=0;i<n;i++)
    {
        p[i].pid = i+1;
        file >> p[i].arrival >> p[i].burst >> p[i].priority;
    }
}

int main()
{
    vector<Process> p;

    int inputChoice;

    cout << "1 Manual Input\n";
    cout << "2 File Input\n";

    cin >> inputChoice;

    if(inputChoice == 1)
    {
        int n;

        cout << "Enter number of processes: ";
        cin >> n;

        p.resize(n);

        for(int i=0;i<n;i++)
        {
            p[i].pid = i+1;

            cout << "\nProcess " << i+1 << "\n";

            cout << "Arrival Time: ";
            cin >> p[i].arrival;

            cout << "Burst Time: ";
            cin >> p[i].burst;

            cout << "Priority: ";
            cin >> p[i].priority;
        }
    }
    else
    {
        readFromFile(p);
    }

    int choice;

    cout << "\nCPU Scheduling Simulator\n";

    cout << "1 FCFS\n";
    cout << "2 SJF\n";
    cout << "3 Priority\n";
    cout << "4 Round Robin\n";
    cout << "5 SRTF\n";
    cout << "6 Multilevel Queue\n";
    cout << "7 Adaptive Scheduler\n";
    cout << "8 Compare Algorithms\n";

    cin >> choice;

    if(choice == 1) fcfs(p);
    else if(choice == 2) sjf(p);
    else if(choice == 3) priorityScheduling(p);
    else if(choice == 4) roundRobin(p,2);
    else if(choice == 5) srtf(p);
    else if(choice == 6) multilevelQueue(p);
    else if(choice == 7)
    {
        int algo = adaptiveChoice(p);

        if(algo == 1) fcfs(p);
        else if(algo == 2) sjf(p);
        else if(algo == 3) priorityScheduling(p);
        else roundRobin(p,2);
    }
    else if(choice == 8)
        compareAlgorithms(p);

    return 0;
}