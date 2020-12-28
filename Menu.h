#ifndef PCB_MENU_H
#define PCB_MENU_H

#include "PCB.h"

class Menu//菜单界面
{
private:
    PCB pcb;
public:
    Menu()//主菜单
    {
        int n, timePiece;
        while (true)
        {
            cout << "请输入资源池长度：";
            cin >> n;
            if (n > 0) break;
            message("输入错误");
        }
        cout << endl;
        while (true)
        {
            cout << "请输入时间片长度：";
            cin >> timePiece;
            if (n > 0) break;
            message("输入错误");
        }
        cout << endl;
        pcb = PCB(n, timePiece);
        while (true)
        {
            int x;
            cout << "主菜单" << endl;
            pcb.showTime();
            cout << "1.时间片到" << endl;
            cout << "2.创建进程" << endl;
            cout << "3.撤销进程" << endl;
            cout << "4.阻塞进程" << endl;
            cout << "5.挂起进程" << endl;
            cout << "6.唤醒进程" << endl;
            cout << "7.激活进程" << endl;
            cout << "8.查看进程" << endl;
            cout << "9.退出" << endl;
            cout << "请选择：";
            cin >> x;
            cout << endl;
            switch (x)
            {
                case 1:pcbControl(); break;
                case 2:create(); break;
                case 3:revoke(); break;
                case 4:block(); break;
                case 5:hang(); break;
                case 6:wakeUp(); break;
                case 7:activate(); break;
                case 8:show(); break;
                case 9:exit(1);
                default:message("输入错误"); break;
            }
        }
    }
    static bool confirm()
    {
        while (true)
        {
            char temp;
            cout << "确认选择（Y/N）" << endl;
            cout << "请输入：";
            cin >> temp;
            cout << endl;
            switch (temp)
            {
                case 'Y':return true;
                case 'N':return false;
                default:
                {
                    message("输入错误");
                    break;
                }
            }
        }
    }
    static void message(const string& message)//显示提示信息
    {
        cout << endl << message << endl << endl;
    }
    void pcbControl()//时间片到
    {
        int n;
        while (true)
        {
            cout << "*时间片到*" << endl;
            pcb.showTimePiece();
            cout << "请输入要跳过的时间片数：";
            cin >> n;
            if (n >= 0) break;
            message("输入错误");
        }
        for (int i = 0; i < n; i++) pcb.pcbControl(true);
        cout << endl;
    }
    void create()//创建进程
    {
        int n;
        while (true)
        {
            cout << "*创建进程*" << endl;
            pcb.showSource();
            if (!pcb.canCreate())
            {
                message("资源池无空闲资源，无法创建进程");
                return;
            }
            cout << "要创建的进程数：";
            cin >> n;
            if (!n)
            {
                cout << endl;
                return;
            }
            if (pcb.canCreate(n) && n > 0) break;
            message("输入错误");
        }
        cout << "请输入进程信息" << endl;
        for (int i = 0; i < n; i++)
        {
            string ID;
            string name;
            int needTime;
            while (true)
            {
                cout << i + 1 << "." << endl;
                cout << "进程标识符：";
                cin >> ID;
                if (pcb.isExistent(ID))
                {
                    message("该进程已存在");
                    continue;
                }
                cout << "进程名：";
                cin >> name;
                cout << "所需完成时间：";
                cin >> needTime;
                if (needTime <= 0)
                {
                    message("输入错误");
                    continue;
                }
                break;
            }
            pcb.create(ID, name, needTime);
        }
        message("创建成功");
    }
    void revoke()//撤销进程
    {
        while (true)
        {
            int x, i = -1;
            cout << "*撤销进程*" << endl;
            cout << "1.正在运行的进程" << endl;
            cout << "2.就绪队列" << endl;
            cout << "3.等待队列" << endl;
            cout << "4.挂起就绪队列" << endl;
            cout << "5.挂起等待队列" << endl;
            cout << "6.退出" << endl;
            cout << "请选择：";
            cin >> x;
            cout << endl;
            switch (x)
            {
                case 1:pcb.showRunning(); break;
                case 2:pcb.showReady(); break;
                case 3:pcb.showWait(); break;
                case 4:pcb.showHangReady(); break;
                case 5:pcb.showHangWait(); break;
                case 6:return;
                default:
                {
                    message("输入错误");
                    continue;
                }
            }
            if (x >= 2 && x <= 5)
            {
                while (true)
                {
                    cout << "请选择要撤销的进程的序号：";
                    cin >> i;
                    if (confirm())
                    {
                        if (pcb.revoke(x, i)) message("撤销成功");
                        else message("输入错误");
                    }
                    break;
                }
            }
            else if (pcb.isRunning()) if (confirm()) pcb.revoke(x, i);
        }
    }
    void block()//阻塞进程
    {
        cout << "*阻塞进程*" << endl;
        pcb.showRunning();
        if (pcb.isRunning()) if (confirm()) pcb.block();
    }
    void hang()//挂起进程
    {
        while (true)
        {
            int x, i = -1;
            cout << "*挂起进程*" << endl;
            cout << "1.就绪队列" << endl;
            cout << "2.等待队列" << endl;
            cout << "3.退出" << endl;
            cout << "请选择：";
            cin >> x;
            cout << endl;
            switch (x)
            {
                case 1:pcb.showReady(); break;
                case 2:pcb.showWait(); break;
                case 3:return;
                default:
                {
                    message("输入错误");
                    continue;
                }
            }
            while (true)
            {
                cout << "请选择要挂起的进程的序号：";
                cin >> i;
                if (confirm())
                {
                    if (pcb.hang(x, i)) message("挂起成功");
                    else message("输入错误");
                }
                break;
            }
        }
    }
    void wakeUp()//唤醒进程
    {
        while (true)
        {
            int i;
            cout << "*唤醒进程*" << endl;
            cout << "0.退出" << endl;
            pcb.showWait();
            cout << "请选择：";
            cin >> i;
            if (!i)
            {
                cout << endl;
                break;
            }
            if (confirm())
            {
                if (pcb.wakeUp(i)) message("唤醒成功");
                else message("输入错误");
            }
        }
    }
    void activate()//激活进程
    {
        while (true)
        {
            int x, i = -1;
            cout << "*激活进程*" << endl;
            cout << "1.挂起就绪队列" << endl;
            cout << "2.挂起等待队列" << endl;
            cout << "3.退出" << endl;
            cout << "请选择：";
            cin >> x;
            cout << endl;
            switch (x)
            {
                case 1:pcb.showHangReady(); break;
                case 2:pcb.showHangWait(); break;
                case 3:return;
                default:
                {
                    message("输入错误");
                    continue;
                }
            }
            while (true)
            {
                cout << "请选择要激活的进程的序号：";
                cin >> i;
                if (confirm())
                {
                    if (pcb.activate(x, i)) message("激活成功");
                    else message("输入错误");
                }
                break;
            }
        }
    }
    void show()//查看进程
    {
        cout << endl;
        pcb.show();
        cout << endl;
    }
};

#endif