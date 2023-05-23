#include <iostream>
#include <thread>

using namespace std;

void thread_function(int x)
{
    cout << "T:" << x << " thread function executing" << endl;
    for (int i = 0; i < 5; i++)
    {
        cout << "T:" << x << " Step: " << i << endl;
    }
}

int main()
{

    thread t1(thread_function, 1);
    thread t2(thread_function, 2);

    cout << "Main thread executing" << endl;
    t1.join();
    t2.join();

    return 0;
}