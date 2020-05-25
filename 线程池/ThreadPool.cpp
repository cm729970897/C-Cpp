#include "ThreadPool.h"

ThreadPool::ThreadPool():
    done(false),isEmpty(true),isFull(false)
{
    
}

void ThreadPool::setSize(int num)
{
    (*this).initnum = num;
}

void ThreadPool::addTask(const Task& f)
{
    if(!done)
    {
        unique_lock<mutex> lk(_mutex);  
        while (isFull)
        {
            cond.wait(lk);
            /* code */
        }
        
        task.push(f);

        if(task.size() == initnum)
            isFull = true;

        cout<<"add a task"<<endl;
        isEmpty = false;

        cond.notify_one(); 
    }
}

void ThreadPool::finish()
{
    for(size_t i = 0; i < threads.size(); ++i)
    {
        threads[i].join();
    }
}


void ThreadPool::runTask()
{
    // 不断判断队列，有任务就执行
    while (!done)
    {
        unique_lock<mutex> lk(_mutex);

        while(isEmpty)
            cond.wait(lk);   

        Task ta;
        ta =move(task.front());
        task.pop();
        
        if(task.empty())
        {
            isEmpty = true;
        }

        isFull = false;
        ta();

        cond.notify_one();
    }
}

void ThreadPool::start(int num)
{

    setSize(num);

    for(int i = 0; i < num; ++i)
    {
        threads.push_back(thread(&ThreadPool::runTask, this));
    }
}

ThreadPool::~ThreadPool()
{

}

