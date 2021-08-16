//srtf.cpp
//Peter Heitkemper
//4/20/20

//Shortest Remaining Time First Scheduling algorithm

#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class PCB {
private:
  int id, burst, arrival, totalBurst;

public:
  PCB(int id, int bt, int at) :id{ id }, burst{ bt }, arrival{ at }, totalBurst{ bt }{}

  int getID() { return id; }
  int getBT() { return burst; }
  int getAT() { return arrival; }
  int getTBT() { return totalBurst; }

  //simulated processing on CPU with a time quatntum tq and process p
  void simCPU() {
    burst -= 1;
    //cout << "     Burst on " << id << endl;
  }
};

bool compare(PCB a, PCB b);
bool compBT(PCB a, PCB b);
void printPCB(PCB p, int arr[]);
void metric(PCB p, int arr[]);
int turnAround(PCB p, int arr[]);
int waiting(PCB p, int arr[]);

string id, bt, at;
int toDo = 0;
int t = 0;
vector<PCB> vin;
int ttime[100];

vector<PCB> jobs;
queue<PCB> done;
queue<PCB> xOrder;


int main(int argc, char* argv[]) {
#include <algorithm>
  string fileName;
  if(argc == 2) {
    fileName = argv[1];
    //cout << "File to Run: " << fileName << endl;
  } else {
    cout << "ERROR: invalid input" << endl;
    return -1;
  }

  //reading in file
  ifstream file(fileName);
  if (!file.is_open()) {
    cout << "ERROR: cannot open " << fileName << endl;
    return -1;
  }
  //extract all data
  while (file.good())
  {
    getline(file, id, ',');
    getline(file, bt, ',');
    getline(file, at);
   // cout << id << "," << bt << ',' << at << endl;
    vin.push_back(PCB(stoi(id), stoi(bt), stoi(at)));
    toDo++;
  }
  //sort by arrival time
  sort(vin.begin(), vin.end(), compare);

  while (toDo > 0) {
    //simulate arrival of process
    //cout << "time: " << t << endl;
    while (!vin.empty() && vin.back().getAT() == t) {
        //cout << "enter new task " << vin.back().getID() << endl;
        jobs.push_back(vin.back());
        vin.pop_back();
        //sort every time new task enters
        sort(jobs.begin(), jobs.end(), compBT);
    }

    if (!jobs.empty()) {
      jobs.front().simCPU();
      xOrder.push(jobs.front());
    }
    t++;

    //check if process is complete
    if (!jobs.empty()&&jobs.front().getBT() == 0) {
      //cout << "task " << jobs.front().getID() << " done" << endl;
      toDo--;
      done.push(jobs.front());
      //cout << "Termination time:          " << t << endl;
      ttime[jobs.front().getID()] = t;
      jobs.erase(jobs.begin()); //process done
    }

  }

  //All Processes complete

  float totalTA = 0;
  float totalP = 0;
  float totalWa = 0;
  while (!done.empty()) {
    totalP++;
    totalTA += turnAround(done.front(), ttime);
    totalWa += waiting(done.front(), ttime);
    done.pop();
  }
  float avgTA = totalTA / totalP;
  float avgWa = totalWa / totalP;
  cout << "SRTF Metrics: " << endl;
  cout << "Average Turnaround Time: " << avgTA << endl;
  cout << "Average Waiting Time: " << avgWa << endl;

  cout << "Process execution order as ID(Termination time): \n";
  while (!xOrder.empty()) {
    cout << xOrder.front().getID() << "(" << ttime[xOrder.front().getID()] << ")";
    int id = xOrder.front().getID();
    while (!xOrder.empty() && id == xOrder.front().getID()) {
      xOrder.pop();
    }
    if (!xOrder.empty()) {
      cout << ", ";
    }else{
	  cout << endl;
	}
  }
  return 0;
}

void printPCB(PCB p, int arr[]) {
  cout << "ID: " << p.getID() << endl;
  cout << "Total Burst Time: " << p.getTBT() << endl;
  cout << "Arrival Time: " << p.getAT() << endl;
  cout << "Termination Time " << arr[p.getID()] << endl;
}

void metric(PCB p, int arr[]) {
  int turnAround = (arr[p.getID()] - p.getAT());
  int waiting = (turnAround - p.getTBT());
  cout << "Metrics for Process ID: " << p.getID() << endl;
  cout << "Turnaround time: " << turnAround << endl;
  cout << "Waiting time: " << waiting << endl;
}
int turnAround(PCB p, int arr[]) {
  int turnAround = (arr[p.getID()] - p.getAT());
  return turnAround;
}
int waiting(PCB p, int arr[]) {
  int turnAround = (arr[p.getID()] - p.getAT());
  int waiting = (turnAround - p.getTBT());
  return waiting;
}

//sort input by Arival time
bool compare(PCB a, PCB b) {
  if (a.getAT() == b.getAT()) {
    return (a.getID() > b.getID()) ? true : false; //smaller ID goes first
  }
  else if (a.getAT() > b.getAT()) {
    return true;
  }
  else {
    return false;
  }
}

//sort by smallest BT
bool compBT(PCB a, PCB b) {
  if (a.getBT() == b.getBT()) {
    return (a.getID() < b.getID()) ? true : false; //smaller ID goes first
  }
  else if (a.getBT() < b.getBT()) {
    return true;
  }
  else {
    return false;
  }
}

