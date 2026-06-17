#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <thread>
#include <chrono>
#include <queue>
using namespace std;
class Cache;
int setLatency(Cache* head,int memIndex,int& ramAccess);
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
bool fetchMemory(Cache& head,int cycle,int& ramAccess){
    if(remainingTime>0){
            remainingTime--;
            return false;
        }
    if(memfetch<memLocation.size()){ 
            cout<<"\nCycle "<<cycle<<" - Running: "<<name<<" Requesting: M"<<memLocation[memfetch]<<"\n";
            remainingTime=setLatency(&head,memLocation[memfetch], ramAccess);
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
    deque<int> recentMem;
    map<int,int> mem;
    public:
    int latency;
    Cache* next=nullptr;
    Cache(string cname,int size,int lat){
        name=cname;
        capacity=size;
        latency=lat;
    }
    int addMemory(int memIndex){
        int evicted=-1;
        if(mem[memIndex]>0){
            return -1;
        }
        if(recentMem.size()>=capacity){
            cout<<"(M"<<recentMem.front()<<" evicted) ";
            evicted = recentMem.front();
            mem.erase(recentMem.front());
            recentMem.pop_front();
        }
        recentMem.push_back(memIndex);
        mem[memIndex]++;
        return evicted;
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
    void printCache() {
        cout<<name<<": [";
        for(int i=0;i<recentMem.size();i++) {
            cout<<"M"<<recentMem[i];
            if(i!=recentMem.size()-1){
                cout<<", ";
            }
        }
        cout<<"] ";
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
int setLatency(Cache* head,int memIndex,int& ramAccess){
    int latency=0;
    Cache* current=head;
    bool hit=false;
    while(current!=nullptr) {
        current->printCache();
        if(current->check(memIndex)){
            cout<<"-> HIT ("<<current->latency<<" cycles)\n";
            latency+=current->latency;
            hit=true;
            break;
        }else{
            cout<<">> MISS\n";
            latency+=current->latency;
            current=current->next;
        }
    }
    if(!hit){
        cout<<"Fetching from RAM\n";
        latency+=200;
        ramAccess++;
    }
    current=head;
    while(current!=nullptr) {
        int evicted=current->addMemory(memIndex);
        current->printCache();
        if(evicted!=-1){
            cout<<"(M"<<evicted<<" evicted)";
        }
        cout<<"\n";
        current=current->next;
    }
    return latency;
}
int main(){
    TaskScheduler ts;
    Cache L1("L1",32,4);
    Cache L2("L2",128,12);
    Cache L3("L3",512,40);
    L1.next=&L2;
    L2.next=&L3;
    int clockcycle=0;
    int ramAccess=0;
    int tasksComplete=0;
    vector<Task> tasks=parseTasks("input_task2.txt");
    
    while(clockcycle<tasks.size()||!ts.empty()){
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        if(clockcycle<tasks.size()){
        ts.addTask(tasks[clockcycle]);
        }
        if(!ts.empty()){
            Task current=ts.pop(); 
            bool isFinished=current.fetchMemory(L1,clockcycle,ramAccess);
            if (isFinished) {
                cout<<"\nCycle: "<<clockcycle<<" -> Task "<<current.name<<" completed"<<endl;
                tasksComplete++;
            } else {
                ts.addTask(current);
            }
        }
        clockcycle++;
    }
    cout<<"\n===Final Results===\n";
    cout<<"Total Cycles: "<<clockcycle<<"\n";
    cout<<"Tasks Completed: "<<tasksComplete<<"\n";
    cout<<"Scheduler:Shortest Remaining Time First(SRTF)\n";
    cout<<"RAM Accesses: "<<ramAccess<<"\n";
    return 0;
}