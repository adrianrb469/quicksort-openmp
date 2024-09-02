#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <omp.h>

#define INPUT_FILE "input.csv"
#define OUTPUT_FILE "output.csv"

void writeRandomNumbers(const std::string& filename, int count) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open " << filename << " for writing." << std::endl;
        exit(1);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);

    for (int i = 0; i < count; ++i) {
        file << dis(gen);
        if (i < count - 1) file << ",";
    }
}

std::vector<int> readNumbers(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open " << filename << " for reading." << std::endl;
        exit(1);
    }

    std::vector<int> numbers;
    std::string value;
    while (std::getline(file, value, ',')) {
        numbers.push_back(std::stoi(value));
    }
    return numbers;
}

void writeNumbers(const std::string& filename, const std::vector<int>& numbers) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open " << filename << " for writing." << std::endl;
        exit(1);
    }

    for (size_t i = 0; i < numbers.size(); ++i) {
        file << numbers[i];
        if (i < numbers.size() - 1) file << ",";
    }
}

int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        #pragma omp task shared(arr) if(high - low > 10000)
        quickSort(arr, low, pi - 1);
        #pragma omp task shared(arr) if(high - low > 10000)
        quickSort(arr, pi + 1, high);
    }
}

void parallelQuickSort(std::vector<int>& arr) {
    #pragma omp parallel
    {
        #pragma omp single nowait
        quickSort(arr, 0, arr.size() - 1);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_elements>" << std::endl;
        return 1;
    }

    int n = std::stoi(argv[1]);

    auto start = std::chrono::high_resolution_clock::now();

    writeRandomNumbers(INPUT_FILE, n);
    std::vector<int> numbers = readNumbers(INPUT_FILE);
    
    parallelQuickSort(numbers);
    
    writeNumbers(OUTPUT_FILE, numbers);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "Time taken: " << diff.count() << " seconds" << std::endl;

    return 0;
}