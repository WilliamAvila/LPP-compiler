#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <algorithm>
#include <fstream>

using namespace std;

mutex m;
string *msg;
string message = "";
string exec(const char *cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), 128, pipe.get()) != NULL)
            result += buffer.data();
    }
    cout << result;
    return result;
}
void in()
{
    while (true)
    {
      
        for (string line; getline(cin, line);)
        {
            m.lock();

            msg = new string();

            *msg += line;
            //*msg += result;
            message+= line+" ";

            m.unlock();
        }
    }
}

void out()
{
    while (true)
    {
        m.lock();
        if (msg)
        {
            ofstream myfile;
            myfile.open ("test1.txt", ofstream::out | ofstream::trunc);
            myfile << message;
            myfile.close();
            message="";
            string command = "./expr < test1.txt";
            string result = exec(command.c_str());
            cout << result;
            msg = 0;
        }
        m.unlock();
        usleep(1000000);
    }
}

int main()
{
    // Disable input/output buffering.
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);

    thread inThread(in);
    thread outThread(out);

    inThread.join();
    outThread.join();

    return 0;
}