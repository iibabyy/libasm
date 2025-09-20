#include <iostream>

using namespace std;

extern "C" int ft_strlen(string str);

int main() {
	cout << "The result is: " << ft_strlen("hahaha") << endl;

	return 0;
}