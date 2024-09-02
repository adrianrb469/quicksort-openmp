#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <algorithm>

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

int* readNumbers(const std::string& filename, int& count) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open " << filename << " for reading." << std::endl;
        exit(1);
    }

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    count = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), ',') + 1;
    file.seekg(0, std::ios::beg);

    int* numbers = new int[count];
    std::string value;
    int i = 0;
    while (std::getline(file, value, ',')) {
        numbers[i++] = std::stoi(value);
    }
    return numbers;
}

void writeNumbers(const std::string& filename, const int* numbers, int count) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open " << filename << " for writing." << std::endl;
        exit(1);
    }

    for (int i = 0; i < count; ++i) {
        file << numbers[i];
        if (i < count - 1) file << ",";
    }
}

int partition(int* arr, int low, int high) {
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

void quickSort(int* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
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
    
    int count;
    int* numbers = readNumbers(INPUT_FILE, count);
    
    quickSort(numbers, 0, count - 1);
    
    writeNumbers(OUTPUT_FILE, numbers, count);

    delete[] numbers;  // Clean up the dynamically allocated memory

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "Time taken: " << diff.count() << " seconds" << std::endl;

    return 0;
}