//
//  Submitted by Michael Oiknine - 40024934
//
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <thread>

// Chrono settings to time functions
using Time = std::chrono::system_clock;
using double_sec = std::chrono::duration<double>;
std::chrono::time_point<Time> start, end;

void quicksort(std::vector<int>&, int, int);
int partition (std::vector<int>&, int, int, int);
int find_pivot(int, int);

// Print the vector to the screen
void print(std::vector<int> v) {
    for (const auto i : v) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

// Swap takes a vector by reference and two positions and then swaps
// both indices within the list
void swap(std::vector<int>& v, int a, int b) {
    int tmp = v[a];
    v[a] = v[b];
    v[b] = tmp;
}

// Read integers for file `input.txt` into a vector and return it
std::vector<int> parse_file() {
    // Create file handle for input
    std::fstream file("input.txt");
    if (!file) {
        std::cerr << "Error opening input file " << std::endl;
        exit(1);
    }

    std::vector<int> res;

    // Read vector size input
    std::string size;
    std::getline(file, size);
    int i_size = std::stoi(size);

    // Read numbers into vector
    std::string num;
    for (; i_size > 0; --i_size) {
        std::getline(file, num);
        res.push_back(std::stoi(num));
    }

    // Release file handle
    file.close();
    return res;
}

// Write the final sorted list to an output file
void write_results(std::vector<int> res) {
    // Create file handle for input
    std::ofstream file("output.txt");
    if (!file) {
        std::cerr << "Error opening output file " << std::endl;
        exit(1);
    }

    // Write each number to the file on a seperate line
    for (int i = 0; i < res.size(); ++i) {
        file << res[i] << "\n";
    }

    // Release file handle
    file.close();
}

int main() {
    // Uncomment to generate random vector for testing purposes
    //std::vector<int> v(33000);
    //std::generate(v.begin(), v.end(), std::rand);

    std::vector<int> v = parse_file();

    start = Time::now();
    std::thread t(quicksort, std::ref(v), 0, v.size() - 1);
    t.join();
    end = Time::now();

    write_results(v);
    std::cout << "Sorted " << v.size() << " numbers in " << double_sec((end - start)).count() << " seconds" << std::endl;

    return 0;
}


// Quicksort is a recursive function which receives a list of numbers
// as well as the list bounds for the current iteration of quicksort
void quicksort(std::vector<int>& v, int left, int right) {
    // Base case
    if (left >= right) {
        return;
    }

    // Get pivot using super advanced algorithm
    int p_index = find_pivot(left, right);

    // Place pivot at the end of the list
    swap(v, p_index, right);

    // Split array into two sub arrays
    int k = partition(v, left, right - 1, right);

    // Call quicksort on the right and left subarray in individual threads
    std::thread rt(quicksort, std::ref(v), k + 1, right);
    std::thread lt(quicksort, std::ref(v), left, k - 1);

    rt.join();
    lt.join();
}

// Partition receives a list, the bounds of the list as well as the current pivot.
// It then shifts all elements greater than the pivot to the right side of the pivot
// and shifts all the elements less than the pivot to the left of the pivot.
//
// The function will return the location of the pivot (where the array is split)
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

// find_Pivot simply returns the the middle index of the list as the pivot
inline int find_pivot(int left, int right) {
    return (left + right) / 2;
}
