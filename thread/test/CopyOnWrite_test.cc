#include "../Mutex.h"
#include "../Thread.h"
#include <vector>
#include <boost/shared_ptr.hpp>
#include <stdio.h>

using namespace muduo;

class Foo
{
 public:
  void doit() const;
};

typedef std::vector<Foo> FooList;
typedef boost::shared_ptr<FooList> FooListPtr;
FooListPtr g_foos;
MutexLock mutex;

void post(const Foo& f)
{
  printf("post\n");
  MutexLockGuard lock(mutex);
  if (!g_foos.unique())
  {
    g_foos.reset(new FooList(*g_foos)); //复制当前容器内容创建新容器，再让共享指针指向新容器
    printf("copy the whole list\n");
  }
  assert(g_foos.unique()); //新的为 1 写入数据。另一份通过 shared_ptr 自动管理生命周期释放。
  g_foos->push_back(f);
}

void traverse()
{
  FooListPtr foos;
  {
    MutexLockGuard lock(mutex); //多线程读写 shared_ptr ,必须用 mutex 保护
    foos = g_foos;
    assert(!g_foos.unique());
  }

  // assert(!foos.unique()); this may not hold

  for (std::vector<Foo>::const_iterator it = foos->begin();
      it != foos->end(); ++it)
  {
    it->doit();
  }
}

void Foo::doit() const
{
  Foo f;
  post(f);
}

int main()
{
  g_foos.reset(new FooList);
  Foo f;
  post(f);
  traverse();
}

