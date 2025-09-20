
#include <iostream>
using namespace std;

extern "C" int ft_strlen(string str);
extern "C" char *ft_strcpy(char *dst, char *src);

int main() {
	char dst[10] = {0};
	char src[] = "Salut";

	cout << "dst (" << &dst << "): " << dst << endl;
	cout << "src (" << &src << "): " << src << endl << endl;

	cout << "Strcpy..." << endl << endl;

	cout << "dst (" << &dst << "): " << dst << endl;
	cout << "src (" << &src << "): " << src << endl;

	return 0;
}