# Multithreading in CPP

## Overview and Explanation

Multithreading is a way to run multiple threads concurrently. A thread is a sequence of instructions within a program that can be executed independently of other code. This is a way to achieve parallelism in a program, and it can be used to utilize multiple cores on a CPU.

## Creating a Thread

In C++, a thread can be created by using the `std::thread` class. The constructor for this class takes a function as an argument. This function will be executed by the thread. The following code creates a thread that executes the `threadFunction` function:

```cpp
void threadFunction() {
    std::cout << "Hello from thread!" << std::endl;
}

int main() {
    std::thread t(threadFunction);
    t.join();
    return 0;
}
```

The `join()` function is used to wait for the thread to finish executing. If this function is not called, the program will terminate before the thread has a chance to finish executing.

---
## How to Compile

To compile the code, run the following command:

```bash 
g++ -pthread main.cpp -o main
or
g++ main.cpp -o main -pthread -std=c++11
```

The `-pthread` flag is used to link the pthread library. The `-std=c++11` flag is used to enable C++11 features.

---
## References

- [C++ Multithreading Tutorial](https://www.geeksforgeeks.org/multithreading-in-cpp/)


## Author

- Lalit Gupta