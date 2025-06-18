#include <iostream>

// write your mult function template here
template <typename T>
T mult(T m1, int m2){return m1 * m2;}

int main()
{
	std::cout << mult(2, 3) << '\n';
	std::cout << mult(1.2, 3) << '\n';

	return 0;
}
