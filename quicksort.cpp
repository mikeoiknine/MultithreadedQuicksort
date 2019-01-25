#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <chrono>

// chrono stuff
using Time = std::chrono::system_clock;
using double_sec = std::chrono::duration<double>;
std::chrono::time_point<Time> start, end;


void quicksort(std::vector<int>&, int, int);
int partition(std::vector<int>&, int, int, int);
int find_pivot(std::vector<int>, int, int);

void print(std::vector<int> v) {
	for (const auto i : v) {
		std::cout << i << " ";
	}
	std::cout << std::endl;
}

void swap(std::vector<int>& v, int a, int b) {
	// printf("Swapped indexes %d and %d\n", a, b);
	int tmp = v[a];
	v[a] = v[b];
	v[b] = tmp;
}

int main() {
	std::vector<int> v(10);
	std::generate(v.begin(), v.end(), std::rand);

	print(v);
	start = Time::now();
	quicksort(v, 0, v.size() - 1);
	end = Time::now();
	print(v);

	std::cout << "Sorted " << v.size() << " numbers in " << double_sec((end - start)).count() << " seconds" << std::endl;
	return 0;
}


// Quicksort is a recursive function which receives a list of numbers 
// as well as the list bounds for the current iteration of quicksort
void quicksort(std::vector<int>& v, int left, int right) {
	//printf("Quicksort! Left: %d, Right: %d\n", left, right);
	// base case
	if (left >= right) {
		return;
	}

	int p_index = find_pivot(v, left, right);
	
	// Place pivot at the end of the list
	swap(v, p_index, right);

	int k = partition(v, left, right - 1, right);
	
	quicksort(v, k + 1, right);
	quicksort(v, left, k - 1);
}

// Partition receives a list, the bounds of the list as well as the current pivot.
// It then shifts all elements greater than the pivot to the right side of the pivot
// and shifts all the elements less than the pivot to the left of the pivot.
//
// The function will return the location the current pivot has been assigned to
int partition(std::vector<int>& v, int left, int right, int p_index) {
	while (left <= right) {
		// Move left iterator towards the right
		while (left <= right && v[left] <= v[p_index]) {
			++left;
		}

		// Move right iterator towards the left
		while (right >= left && v[right] > v[p_index]) {
			--right;
		}
		
		// If the iterators did not cross, a swap must be made
		if (left < right) {
			swap(v, left, right);
		}
	}

	// Put pivot in place
	swap(v, left, p_index);
	return left;
}

// TODO: explain media of three magic
inline int find_pivot(std::vector<int> v, int left, int right) {
	int mid = (left + right) / 2;
	return mid;
}
