#pragma once

class Routines
{
public:
	template <typename T>
	static void swap(T& a, T& b) {
		T temp = b;
		b = a;
		a = temp;
	}
};