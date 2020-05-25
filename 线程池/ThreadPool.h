/*
1. 链表或数组：用于存储线程中的线程
2. 队列: 用于存储需要放入线程池中执行的任务
3. 条件变量：当有任务需要执行时，用于通知正在等待的线程从任务队列，
*/
#ifndef THREAD_POLL_H
#define THREAD_POLL_H

#include <iostream>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <functional>
#include <queue>

#define N 10

using namespace std;

class ThreadPool
{
public:
  typedef function<void()>Task;
  
  ThreadPool();
  ~ThreadPool();

  void addTask(const Task& f);
  void start(int num);
  void setSize(int num);
  void runTask();
  void finish();

private:
  size_t initnum;
  
  vector<thread> threads;

  queue<Task> task;

  mutex _mutex;
  condition_variable cond;
  
  bool done; // 线程池是否结束
  bool isEmpty; // 队列是否为空
  bool isFull; // 队列是否为满
};

#endif
