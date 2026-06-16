#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <thread>
#include <chrono>
using namespace std;
class Cache;
int setLatency(Cache* head,int memIndex);
class Task{
    int memfetch=0;
    int remainingTime=0;
    public:
string name;
int burst;
vector<int> memLocation;
Task(string taskName,int burstTime,vector<int> mem){
name=taskName;
burst=burstTime;
memLocation=mem;
} 
bool fetchMemory(Cache& head){
    if(remainingTime>0){
            remainingTime--;
            return false;
        }
    if(memfetch<memLocation.size()){ 
            remainingTime=setLatency(&head,memLocation[memfetch]);
            memfetch++;
            remainingTime--;
            return false;
    }
        if(burst>0){
            burst--;
            return false;
        }else{
            return true;
        }
    
}
};
class Cache{
    string name;
    int capacity;
    queue<int> recentMem;
    map<int,int> mem;
    public:
    int latency;
    Cache* next=nullptr;
    Cache(string cname,int size,int lat){
        name=cname;
        capacity=size;
        latency=lat;
    }
    void addMemory(int memIndex){
        if(mem[memIndex]>0){
            return;
        }
        if(recentMem.size()>=capacity){
            mem.erase(recentMem.front());
            recentMem.pop();
        }
        recentMem.push(memIndex);
        mem[memIndex]++;
    }
    void addMemory(Task t){
       for(int i=0;i<t.memLocation.size();i++){
        addMemory(t.memLocation[i]);
    }
    }
    bool check(int memIndex){
        if(mem[memIndex]>0){
            return true;
        }else{
            return false;
        }
    }
   // bool checkMem
};

class TaskComparer{
    public:
    bool operator()(const Task& t1,const Task& t2){
        if(t1.burst==t2.burst){
            return stoi(t1.name.substr(1))>stoi(t2.name.substr(1)); 
        }
        return t1.burst>t2.burst;
    }
};
class TaskScheduler{
   priority_queue<Task,vector<Task>,TaskComparer> tasks;
public:
void addTask(Task t){
tasks.push(t);
}
Task top(){
    return tasks.top();
}
Task pop(){
    Task t=top();
    tasks.pop();
    return t;
}
bool empty(){
    return tasks.empty();
}
};
vector<Task> parseTasks(string filename){
ifstream file(filename);
vector<Task> result;
    string line="";
    while(getline(file,line)){
        string str="";
        vector<string> words;
        for(int i=0;i<line.length();i++){
            if(line[i]!=' '){
                str+=line[i];
            }else{
                words.push_back(str);
                str="";
            }
        }
        if(str!="") {
            words.push_back(str);
        }
        int counter=5;
        vector<int> mem;
        while(counter<words.size()){
            int loc=stoi(words[counter].substr(1));
            mem.push_back(loc);
            counter++;
        }
        Task t(words[1],stoi(words[3]),mem);
        result.push_back(t);
    }
    return result;
}
int setLatency(Cache* head,int memIndex){
    int latency=0;
    while(head!=nullptr&&!head->check(memIndex)){
        latency+=head->latency;
        head->addMemory(memIndex);
        head=head->next;
    }
    if(head==nullptr){
        latency+=200;
    }else{
        latency+=head->latency;
    }
    return latency;
}
int main(){
    cout<<"Hi Awwab(Now in kidnapper[Avanish?] house) or Aryan!"<<endl;
    TaskScheduler ts;
    Cache L1("L1",32,4);
    Cache L2("L2",128,12);
    Cache L3("L3",512,40);
    L1.next=&L2;
    L2.next=&L3;
    vector<Task> tasks=parseTasks("input_task2.txt");
    while(!ts.empty()){
        cout<<ts.top().name<<" "<<ts.top().burst<<endl;
        ts.pop();
    }
    int clockcycle=0;
    while(true){
        cout<<"\rCycle: "<<clockcycle<<flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        if(clockcycle<tasks.size()){
        ts.addTask(tasks[clockcycle]);
        }
        if(!ts.empty()){
            Task current=ts.pop(); 
            bool isFinished=current.fetchMemory(L1);
            if (isFinished) {
                cout<<"\n[Cycle "<<clockcycle<<"] Task "<<current.name<<" completed!"<<endl;
            } else {
                ts.addTask(current);
            }
        }
        clockcycle++;
    }
    return 0;
}