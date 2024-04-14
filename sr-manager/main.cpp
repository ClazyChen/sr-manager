#include "cli/CreatorCli.hpp"

#include <iostream>
#include <windows.h>
using namespace std;
using namespace sr;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    for (int i = 1; ; i++) {
        cout << "第 " << i << " 次测试" << endl;
        CreatorCli cli;
        cli.run();
        getchar();
    }
	return 0;
}