#ifndef PCB_PCB_H
#define PCB_PCB_H

#include <string>
#include <utility>
#include <vector>
#include <iomanip>
#include <iostream>
using namespace std;

class PCBNode//进程结点
{
private:
    string ID;//进程标识符
    string name;//进程名
    int enterTime;//进入时间
    int startTime;//开始运行时间
    int endTime;//结束运行时间
    int needTime;//所需完成时间
    int completeTime;//已经完成时间
    int turnaroundTime;//周转时间
    float weightedTurnaroundTime;//带权周转时间
    bool empty;//判断结点是否为空
public:
    PCBNode()//初始化
    {
        ID = name = "";
        enterTime = startTime = endTime = needTime = completeTime = turnaroundTime = 0;
        weightedTurnaroundTime = 0;
        empty = true;
    }
    PCBNode(string ID, string name, int enterTime, int needTime) :
            ID(std::move(ID)), name(std::move(name)), enterTime(enterTime), needTime(needTime)//初始化
    {
        startTime = endTime = completeTime = turnaroundTime = 0;
        weightedTurnaroundTime = 0;
        empty = false;
    }
    bool isCompleted() const//判断进程是否完成
    {
        if (isEmpty()) return false;
        if (completeTime == needTime) return true;
        return false;
    }
    bool isEmpty() const//判断结点是否为空
    {
        return empty;
    }
    bool isEqual(string ID)//判断输入标识符与此结点标识符是否相同
    {
        if (!this->ID.compare(ID)) return true;
        return false;
    }
    void timeSlice(int time)//时间片过
    {
        completeTime++;
        if (completeTime == needTime)
        {
            endTime = time;
            turnaroundTime = endTime - startTime;
            weightedTurnaroundTime = (float)turnaroundTime / (float)needTime;
        }
    }
    void timeSliceTo(int time)//时间片到
    {
        if (!completeTime) startTime = time;
    }
    void show()//打印进程信息
    {
        cout << "\t" << ID;
        cout << "\t" << "\t" << "\t" << name;
        cout << "\t" << "\t" << enterTime;
        cout << "\t" << "\t" << startTime;
        cout << "\t" << "\t" << "\t" << endTime;
        cout << "\t" << "\t" << "\t" << needTime;
        cout << "\t" << "\t" << "\t" << completeTime;
        cout << "\t" << "\t" << "\t" << turnaroundTime;
        cout << "\t" << "\t" << weightedTurnaroundTime;
        cout << endl;
    }
};

class PCBQueue//进程队列
{
private:
    vector<PCBNode> queue;
public:
    PCBQueue() = default;//初始化
    explicit PCBQueue(int n)//初始化空结点队列
    {
        for (int i = 0; i < n; i++)
        {
            PCBNode pcbNode;
            push(pcbNode);
        }
    }
    bool isEmpty()//判断进程队列是否为空
    {
        return queue.empty();
    }
    bool isExistent(const string& ID)//判断输入标识符是否存在于该队列
    {
        for (int i = 0; i < getSize(); i++) if (queue[i].isEqual(ID)) return true;
        return false;
    }
    int getSize()//返回队列大小
    {
        return queue.size();
    }
    PCBNode pop()//队首弹出结点
    {
        PCBNode pcbNode;
        if (!isEmpty())
        {
            pcbNode = queue[0];
            queue.erase(queue.begin());
        }
        return pcbNode;
    }
    void push()//队尾插入空结点
    {
        PCBNode pcbNode;
        push(pcbNode);
    }
    void push(const PCBNode& pcbNode)//队尾插入结点
    {
        queue.push_back(pcbNode);
    }
    PCBNode revoke(int i)//撤销进程
    {
        PCBNode pcbNode = queue[i];
        queue.erase(queue.begin() + i);
        return pcbNode;
    }
    void show()//打印进程队列信息
    {
        if (isEmpty()) cout << "无" << endl;
        else
        {
            cout << "总进程数：" << getSize() << endl;
            cout << "\t进程标识符\t进程名\t进入时间\t开始运行时间\t结束运行时间\t所需完成时间\t已经完成时间\t周转时间\t带权周转时间" << endl;
            for (int i = 0; i < getSize(); i++)
            {
                cout << i + 1 << ".";
                queue[i].show();
            }
        }
    }
};

class PCB//进程管理
{
private:
    PCBNode running;//正在运行的进程
    PCBQueue ready;//就绪队列
    PCBQueue wait;//等待队列
    PCBQueue hangReady;//挂起就绪队列
    PCBQueue hangWait;//挂起等待队列
    PCBQueue complete;//完成队列
    PCBQueue source;//资源池
    int time;//进程管理时间
    int timePiece;//时间片
public:
    PCB()//初始化
    {
        time = 0;
        timePiece = 1;
    }
    PCB(int n, int timePiece)//初始化
    {
        source = PCBQueue(n);
        time = 0;
        this->timePiece = timePiece;
    }
    bool activate(int x, int i)//激活进程
    {
        i--;
        switch (x)
        {
            case 1:
            {
                if (i < 0 || i >= hangReady.getSize()) return false;
                PCBNode pcbNode = hangReady.revoke(i);
                source.pop();
                ready.push(pcbNode);
                break;
            }
            case 2:
            {
                if (i < 0 || i >= hangWait.getSize()) return false;
                PCBNode pcbNode = hangWait.revoke(i);
                source.pop();
                wait.push(pcbNode);
                break;
            }
            default:return false;
        }
        return true;
    }
    bool canCreate()//判断有无空闲资源创建进程
    {
        if (source.isEmpty()) return false;
        return true;
    }
    bool canCreate(int n)//判断有无空闲资源创建进程
    {
        if (n > source.getSize()) return false;
        return true;
    }
    bool hang(int x, int i)//挂起进程
    {
        i--;
        switch (x)
        {
            case 1:
            {
                if (i < 0 || i >= ready.getSize()) return false;
                PCBNode pcbNode = ready.revoke(i);
                source.push();
                hangReady.push(pcbNode);
                break;
            }
            case 2:
            {
                if (i < 0 || i >= wait.getSize()) return false;
                PCBNode pcbNode = wait.revoke(i);
                source.push();
                hangWait.push(pcbNode);
                break;
            }
            default:return false;
        }
        return true;
    }
    bool isExistent(const string& ID)//判断所有进程队列中是否存在该进程
    {
        if (running.isEqual(ID) || ready.isExistent(ID) || wait.isExistent(ID) || hangReady.isExistent(ID)
            || hangWait.isExistent(ID) || complete.isExistent(ID)) return true;
        return false;
    }
    bool isRunning()//判断是否有进程正在运行
    {
        return !running.isEmpty();
    }
    bool revoke(int x, int i)//撤销进程
    {
        i--;
        switch (x)
        {
            case 1:
            {
                running = PCBNode();
                source.push();
                pcbControl(false);
                break;
            }
            case 2:
            {
                if (i < 0 || i >= ready.getSize()) return false;
                ready.revoke(i);
                source.push();
                break;
            }
            case 3:
            {
                if (i < 0 || i >= wait.getSize()) return false;
                wait.revoke(i);
                source.push();
                break;
            }
            case 4:
            {
                if (i < 0 || i >= hangReady.getSize()) return false;
                hangReady.revoke(i);
                break;
            }
            case 5:
            {
                if (i < 0 || i >= hangWait.getSize()) return false;
                hangWait.revoke(i);
                break;
            }
            default:return false;
        }
        return true;
    }
    bool wakeUp(int i)//唤醒进程
    {
        i--;
        if (i < 0 || i >= wait.getSize()) return false;
        PCBNode pcbNode = wait.revoke(i);
        ready.push(pcbNode);
        return true;
    }
    void block()//阻塞进程
    {
        wait.push(running);
        running = PCBNode();
        pcbControl(false);
    }
    void create(const string& ID, const string& name, int needTime)//创建进程
    {
        PCBNode pcbNode(ID, name, time, needTime);
        source.pop();
        ready.push(pcbNode);
        if (running.isEmpty()) pcbControl(false);
    }
    void pcbControl(bool temp)//时间片到
    {
        int i = 0;
        if (!running.isEmpty())
        {
            for (i = 1; i <= timePiece; i++)
            {
                running.timeSlice(time + i);
                if (running.isCompleted()) break;
            }
            if (i > timePiece) i = timePiece;
            if (running.isCompleted())
            {
                complete.push(running);
                source.push();
            }
            else ready.push(running);
            running = PCBNode();
        }
        if (temp)
        {
            if (!i) time += timePiece;
            else time += i;
        }
        if (!ready.isEmpty())
        {
            running = ready.pop();
            running.timeSliceTo(time);
        }
    }
    void show()//打印所有进程队列信息
    {
        showTime();
        showRunning();
        showReady();
        showWait();
        showHangReady();
        showHangWait();
        showComplete();
        showSource();
    }
    void showRunning()//打印正在运行的进程
    {
        cout << "*正在运行的进程*" << endl;
        if (running.isEmpty()) cout << "无" << endl;
        else
        {
            cout << "\t进程标识符\t进程名\t进入时间\t开始运行时间\t结束运行时间\t所需完成时间\t已经完成时间\t周转时间\t带权周转时间" << endl;
            running.show();
        }
    }
    void showReady()//打印就绪队列
    {
        cout << "*就绪队列*" << endl;
        ready.show();
    }
    void showWait()//打印等待队列
    {
        cout << "*等待队列*" << endl;
        wait.show();
    }
    void showHangReady()//打印挂起就绪队列
    {
        cout << "*挂起就绪队列*" << endl;
        hangReady.show();
    }
    void showHangWait()//打印挂起等待队列
    {
        cout << "*挂起等待队列*" << endl;
        hangWait.show();
    }
    void showComplete()//打印完成队列
    {
        cout << "*完成队列*" << endl;
        complete.show();
    }
    void showSource()//打印资源池
    {
        cout << "*资源池空闲数*" << endl;
        cout << source.getSize() << endl;
    }
    void showTime() const//打印进程管理时间
    {
        cout << "*当前时间*" << endl;
        cout << time << endl;
    }
    void showTimePiece() const//打印时间片长度
    {
        cout << "*时间片长度*" << endl;
        cout << timePiece << endl;
    }
};

#endif