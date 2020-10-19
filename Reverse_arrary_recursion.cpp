#include <iostream>
using namespace std;

void Reverse_array(int* arr, int _size, int size)
{
	if (size > 1)
		Reverse_array(arr, _size, size - 1);
	cout << arr[_size - size] << endl;
}

int main()
{
	int size;
	cin >> size;
	int* arr = new int[size];
	for (int i = 1; i < size + 1; i++)
		arr[i - 1] = i;
	Reverse_array(arr, size, size);
	delete[] arr;
	return 0;
}