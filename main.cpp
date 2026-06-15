#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
using namespace std;
class Task{
    public:
string name;
int burst;
vector<int> memLocation;
Task(string taskName,int burstTime,vector<int> mem){
name=taskName;
burst=burstTime;
memLocation=mem;
} 
};
class TaskScheduler{
   priority_queue<pair<int,Task>,vector<pair<int,Task>>,greater<pair<int,Task>>> tasks;
public:
void addTask(Task t){
tasks.push({t.burst,t});
}
Task top(){
    return tasks.top().second;
}
};
vector<Task> parseTasks(string filename){
ifstream file(filename);
vector<Task> result;
    string line="";
    while(getline(file,line)){
        cout<<line<<endl;
        string str="";
        vector<string> words;
        for(int i=0;i<line.length();i++){
            if(line[i]!=' '){
                str+=line[i];
            }else{
                words.push_back(str);
            }
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
int main(){
    cout<<"Hi Awwab(Still in basement) or Aryan!"<<endl;
    TaskScheduler ts;
    vector<Task> tasks=parseTasks("input_task2.txt");
    for(Task task:tasks){
    ts.addTask(task);
    }
    
    return 0;
}