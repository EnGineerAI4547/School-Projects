#include<iostream> 
#include<bits/stdc++.h>
using namespace std;

typedef pair<int,int> pi;
void sjf_alg(vector<vector<int>> process, priority_queue< pi, vector<pi>, greater<pi> > ready_queue,vector<int> process_burst_index);
void print_completed_processes(vector<int> completed);
void print_queue(priority_queue< pi, vector<pi>, greater<pi> > q, vector<vector<int>> process, vector<int> process_burst_index);
void print_context_switch(vector<vector<int>> process,int process_index,priority_queue< pi, vector<pi>, greater<pi> > ready_queue,vector<int> process_burst_index, int time);


int main(){

    int num_p = 8;
    vector<vector<int>> process(num_p);
// queue <int> readyQueueFCFS;
    priority_queue< pi, vector<pi>, greater<pi> > ready_queue_SJF;
    vector<int> process_burst_index(num_p,0);
    process[0] = {5, 27, 3, 31, 5, 43, 4, 18, 6, 22, 4, 26, 3, 24, 5};
    process[1] = {4, 48, 5, 44, 7, 42, 12, 37, 9, 76, 4, 41, 9, 31, 7, 43, 8};
    process[2] = {8, 33, 12, 41, 18, 65, 14, 21, 4, 61, 15, 18, 14, 26, 5, 31, 6};
    process[3] = {3, 35, 4, 41, 5, 45, 3, 51, 4, 61, 5, 54, 6, 82, 5, 77, 3};
    process[4] = {16, 24, 17, 21, 5, 36, 16, 26, 7, 31, 13, 28, 11, 21, 6, 13, 3, 11, 4};
    process[5] = {11, 22, 4, 8, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8};
    process[6] = {14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10};
    process[7] = {4, 14, 5, 33, 6, 51, 14, 73, 16, 87, 6};

    for(int i=0;i<process.size();i++)
{
// readyQueueFCFS.push(i);
    ready_queue_SJF.push(make_pair(process[i][process_burst_index[i]],i));
        
}
    cout<<endl<<"----SJF----"<<endl;
    sjf(process,ready_queue_SJF,process_burst_index);
}


void sjf(vector<vector<int>> process, priority_queue< pi, vector<pi>, greater<pi> > ready_queue,vector<int> process_burst_index)
{
    
    vector<pair<int,int>> io_return; //array of pair<return time,process id>
    vector<int> arrival_time(process.size(),0);
    vector<int> waiting_time(process.size(),0);
    vector<int> turn_around_time(process.size(),0);
    vector<int> response_time(process.size(),-1);
    vector<int> completed(process.size(),0);

    for(int i=0;i<process.size();i++){
    io_return.push_back(make_pair(INT_MAX,i));
}
    int time = 0;
    int idle_cpu_time=0;
    while(!ready_queue.empty())
{
    
    int process_index = ready_queue.top().second;
    ready_queue.pop();
    
    //waiting time in ready queue
    waiting_time[process_index] += time - arrival_time[process_index];
    
    //takes value of first waiting time and then resets flag so the if condition never executes again
    if(response_time[process_index] == -1)
{
    response_time[process_index] = time - arrival_time[process_index];
}
    print_context_switch(process,process_index,ready_queue,process_burst_index, time);
    time += process[process_index][process_burst_index[process_index]]; // process getting cpu burst time

    if(process_burst_index[process_index] <= process[process_index].size()-3)
{
    for(int i=0;i<process.size();i++)
{
    if(io_return[i].second == process_index)
{
    io_return[i].first = process[process_index][process_burst_index[process_index]+1] + time;
    break;
    
}
}
}
    else{completed[process_index] = 1;
        
}

    print_completed_processes(completed);
    process_burst_index[process_index] += 2;
    sort(io_return.begin(), io_return.end());
    
    if(ready_queue.empty()){
    while(io_return[0].first != INT_MAX && io_return[0].first > time)
{   
    time++;idle_cpu_time++;
    
}
}
    for(int i=0;i<process.size();i++)
{
    if(io_return[i].first <= time)
{
    ready_queue.push(make_pair(process[io_return[i].second][process_burst_index[io_return[i].second]],io_return[i].second)); //pushing process id into readyQueue
    arrival_time[io_return[i].second] = io_return[i].first;
    io_return[i].first = INT_MAX;
}
}
}
    int burst_time = time - idle_cpu_time;
    
    for(int i=0;i<process.size();i++)
{
    int total_burst_time=0;
    for(int j=0;j<process[i].size();j++)
{
    total_burst_time += process[i][j];
}
    turn_around_time[i] += total_burst_time + waiting_time[i];
}

//printing result
    cout<<"******************************************************** "<<endl;
    cout<<"Total time required to complete all the 8 processes: "<<time<<endl;
    cout<<"CPU Utilization: "<<fixed<<setprecision(2)<<(float)burst_time/time*100<<endl<<endl;
    cout<<"******************************************************** "<<endl;
    cout<<"******************************************************** "<<endl;
    cout<<"WAITING TIME"<<endl;
    int total_waiting_time = 0;
    for(int i=0;i<process.size();i++)
{
    cout<<"Process: "<<i<<", Waiting Time: "<<waiting_time[i]<<endl;
    total_waiting_time += waiting_time[i];
}
    cout<<"Average Waiting Time: "<<fixed<<setprecision(2)<<(float)total_waiting_time/process.size()<<endl<<endl;
    cout<<"******************************************************** "<<endl;
    cout<<"******************************************************** "<<endl;   
    cout<<"TURNAROUND TIME"<<endl;
    int total_turn_around = 0;
    for(int i=0;i<process.size();i++)
{
    cout<<"Process: "<<i<<", Turnaround Time: "<<turn_around_time[i]<<endl;
    total_turn_around += turn_around_time[i];
}
    cout<<"Average Turnaround Time: "<<fixed<<setprecision(2)<<(float)total_turn_around/process.size()<<endl<<endl;
    cout<<"******************************************************** "<<endl;
    cout<<"******************************************************** "<<endl;

    cout<<"RESPONSE TIME"<<endl;
    int total_response_time = 0;
    for(int i=0;i<process.size();i++)
{
    cout<<"Process: "<<i<<", Response Time: "<<response_time[i]<<endl;
    total_response_time += response_time[i];
}
    cout<<"Average Response Time: "<<fixed<<setprecision(2)<<(float)total_response_time/process.size()<<endl;
}  
    void print_queue(priority_queue< pi, vector<pi>, greater<pi> > q, vector<vector<int>> process, vector<int> process_burst_index)
{
    //printing content of queue
    cout<<setfill(' ')<<setw(10)<<"Process"<<setw(10)<<"Burst"<<endl;
    if(!q.empty())
{
    while (!q.empty())
{
    int pid = q.top().second;
    cout<<setfill(' ')<<setw(10)<<"P"<<pid<<setw(10)<<process[pid][process_burst_index[pid]]<<endl;
    q.pop();

}
}
    else{
    cout<<setfill(' ')<<setw(10)<<"[empty]"<<setw(10)<<"N/A"<<endl;
}
    cout<<endl;
}
    
    
    
void print_completed_processes(vector<int> completed)
{
    
    cout<<"Completed Processes: ";
    for(int i=0;i<completed.size();i++)
{
    if(completed[i] == 1)
{
    cout<<"P"<<i<<" ";
}
}
    cout<<endl;
    cout<<setfill('-')<<setw(40)<<"-"<<endl;
}
    void print_context_switch(vector<vector<int>> process,int process_index,priority_queue
    < pi, vector<pi>, greater<pi> > ready_queue,vector<int> process_burst_index, int time)
{
    cout<<setfill('-')<<setw(40)<<"-"<<endl;
    cout<<"Current Execution Time: "<<time<<endl<<endl;
    cout<<"Next Process on the CPU: P"<<process_index<<endl;
    cout<<setfill('-')<<setw(40)<<"-"<<endl<<endl;
    cout<<"Ready queue:"<<endl;
    print_queue(ready_queue,process,process_burst_index);
    cout<<setfill('-')<<setw(40)<<"-"<<endl;
    
}
