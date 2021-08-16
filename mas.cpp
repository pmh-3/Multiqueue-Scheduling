//mas.cpp
//Peter Heitkemper
//4-20-21

//Multiqueue Adaptive Scheduling Algorithm

#include <queue>
#include <iostream>
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
void printPCB(PCB p, int arr[]);
void metric(PCB p, int arr[]);
int turnAround(PCB p, int arr[]);
int waiting(PCB p, int arr[]);

string id, bt, at;
int toDo = 0;
int t = 0;
vector<PCB> vin;
int ttime[100];
queue<PCB> q0;
//time quantum in milliseconds
int tq0 = 6;
queue<PCB> q1;
//time quantum in milliseconds
int tq1 = 12;

queue<PCB> q2;
queue<PCB> done;
queue<PCB> xOrder;

int main(int argc, char *argv[]) {
  #include <algorithm>
  string fileName;
  if (argc == 2) {
    fileName = argv[1];
    //cout << "File to Run: " << fileName << endl;
  }
  else {
    cout << "ERROR: invalid input" << endl;
    return -1;
  }
  //reading in file
  ifstream file(fileName);
  if (!file.is_open()) {
    cout << "ERROR: cannot open file";
    return -1;
  }

  //extract all data
  while (file.good())
  {
    getline(file, id, ',');
    getline(file, bt, ',');
    getline(file, at);
    //cout << id << "," << bt << ',' << at << endl;
    vin.push_back(PCB(stoi(id), stoi(bt), stoi(at)));
    toDo++;
  }
  //sort by arrival time
  sort(vin.begin(), vin.end(), compare);

  while (toDo > 0) {
    //simulate arrival of process
    //cout << "time: " << t << endl;
    if (!vin.empty()) {
      while (vin.back().getAT() == t) {
       // cout << "enter new task " << vin.back().getID() << endl;
        q0.push(vin.back());
        vin.pop_back();
        if (vin.empty()) { break; }
      }
    }

  queue0:
    //cout << "Queue 0: " << toDo << endl;
    while(!q0.empty()) {
      //q0.front().printPCB();
      for (int i = 0; i < tq0; i++) {
        q0.front().simCPU();
        xOrder.push(q0.front());
        t++;
        while (!vin.empty()&&vin.back().getAT() == t) {
           // cout << "enter new task " << vin.back().getID() << endl;
            q0.push(vin.back());
            vin.pop_back();
        }
        if (q0.front().getBT() == 0) {
          //cout << "task " << q0.front().getID() << " done" << endl;
          toDo--;
          done.push(q0.front());
          //cout << "Term time:             " << t << endl;
          ttime[q0.front().getID()] = t;
          q0.pop(); //process done
          if (q0.empty()) {
           // cout << "Q0 is empty";
            goto end0;  
          }
          else {
            i = 0; //reset time quant
            goto queue0;
           // cout << "Begin new task " << q0.front().getID() << endl;
          }
        }
      }
      if (q0.front().getBT() > 0) {     //process not finished
        //cout << "Push " << q0.front().getID() << " to q1 \n";
        q1.push(q0.front());
        q0.pop();
      }
    }
  end0:

    //cout << "Queue 1: " << toDo << endl;
    while(!q1.empty()) {
      for (int i = 0; i < tq1; i++) {
        q1.front().simCPU();
        xOrder.push(q1.front());
        t++;
        int flag = 0;
        while (!vin.empty() && vin.back().getAT() == t) {
            //cout << "enter new task " << vin.back().getID() << endl;
            q0.push(vin.back());
            vin.pop_back();
            //cout << "prempt to q0 \n";
            //move current process to end of q
            q1.push(q1.front());
            q1.pop();
            flag = 1;
        }
        if (flag == 1) { goto queue0; }
        //Process done
        if (q1.front().getBT() == 0) {
          //cout << "task " << q1.front().getID() << " done" << endl;
          toDo--;
          done.push(q1.front());
          ttime[q1.front().getID()] = t;
          q1.pop(); //process done
          if (q1.empty()) {
            //cout << "Q1 is empty \n";
            goto end1;
          }
          i = 0; //reset time quant
          goto end0;
          //cout << "Begin new task " << q1.front().getID() << endl;
        }
      }
      if (q1.front().getBT() > 0) {     //process not finished
        //cout << "Push " << q1.front().getID() << " to q2 \n";
        q2.push(q1.front());
        q1.pop();
      }
    }
  end1:

    //cout << "Queue 2: " << toDo << endl;
    while (!q2.empty()) {
      if (!q1.empty()) {
        //cout << "preempt to q1 \n";
        //move current process to end of q
        q2.push(q2.front());
        q2.pop();
        goto end0;
      }
      q2.front().simCPU();
      xOrder.push(q2.front());
      t++;
      int flag = 0;
      if (!vin.empty()) {
        while (!vin.empty() && vin.back().getAT() == t) {
         // cout << "enter new task " << vin.back().getID() << endl;
         // cout << "prempt to q0 \n";
          q0.push(vin.back());
          vin.pop_back();
          //move current process to end of q
          q2.push(q2.front());
          q2.pop();
          flag = 1;
          if (vin.empty()) { break; }
        }
      }
      if (flag == 1) { goto queue0; }
      if (q2.front().getBT() == 0) {
        //cout << "task " << q2.front().getID() << " done" << endl;
        toDo--;
        done.push(q2.front());
        ttime[q2.front().getID()] = t;
        q2.pop(); //process done
      }
    }
    t++;
    //cout << " time: " << t << endl;
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
  cout << "MAS Report: " << endl;
  cout << "Average Turnaround Time: " << avgTA << endl;
  cout << "Average Waiting Time: " << avgWa << endl;

  cout << "Execution order of processes as ID(Termination time): \n";
  while (!xOrder.empty()) {
    cout << xOrder.front().getID() << "(" << ttime[xOrder.front().getID()] << ")";
    int id = xOrder.front().getID();
    while (!xOrder.empty()&& id == xOrder.front().getID()) {
      xOrder.pop();
    }
    if (!xOrder.empty()) {
      cout << ", ";
    }
    else {
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