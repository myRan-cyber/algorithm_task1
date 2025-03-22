#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <limits.h>
using namespace std;

// 生成随机数据文件（1k, 10k, 50k, 100k）
void generate_data(const string& filename, int n) {
    ofstream fout(filename);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(INT_MIN, INT_MAX);

    for (int i = 0; i < n; i++) {
        fout << dist(gen) << "\n";
    }
    fout.close();
}

// 从文件加载数据到vector
vector<int> load_data(const string& filename) {
    ifstream fin(filename);
    vector<int> data;
    int num;
    while (fin >> num) {
        data.push_back(num);
    }
    fin.close();
    return data;
}

// 插入排序
void insertion_sort(vector<int>& arr) {
    for (int i = 1; i < arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 自底向上合并排序
void bottom_up_merge_sort(vector<int>& arr) {
    vector<int> temp(arr.size());
    for (int width = 1; width < arr.size(); width *= 2) {
        for (int i = 0; i < arr.size(); i += 2 * width) {
            int left = i;
            int mid = min(i + width, (int)arr.size());
            int right = min(i + 2 * width, (int)arr.size());
            
            int i1 = left, i2 = mid, k = left;
            while (i1 < mid && i2 < right) {
                temp[k++] = (arr[i1] <= arr[i2]) ? arr[i1++] : arr[i2++];
            }
            while (i1 < mid) temp[k++] = arr[i1++];
            while (i2 < right) temp[k++] = arr[i2++];
            
            for (int j = left; j < right; j++) {
                arr[j] = temp[j];
            }
        }
    }
}

// 选择排序
void selection_sort(vector<int>& arr) {
    for (int i = 0; i < arr.size() - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < arr.size(); j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        swap(arr[i], arr[min_idx]);
    }
}

// 冒泡排序
void bubble_sort(vector<int>& arr) {
    for (int i = 0; i < arr.size() - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < arr.size() - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// 堆排序
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heap_sort(vector<int>& arr) {
    for (int i = arr.size() / 2 - 1; i >= 0; i--)
        heapify(arr, arr.size(), i);

    for (int i = arr.size() - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// 测试排序算法并返回时间（毫秒）
long long test_sort(void (*sort_func)(vector<int>&), vector<int> data) {
    auto start = chrono::high_resolution_clock::now();
    sort_func(data);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(end - start).count();
}

int main() {
    // 生成测试数据
    vector<int> sizes = {1000, 10000, 50000, 100000};
    for (int n : sizes) {
        generate_data("test_" + to_string(n) + ".txt", n);
    }

    // 测试每个算法
    vector<pair<string, void (*)(vector<int>&)>> algorithms = {
        {"Insertion Sort", insertion_sort},
        {"Selection Sort", selection_sort},
        {"Bubble Sort", bubble_sort},
        {"Merge Sort", bottom_up_merge_sort},
        {"Heap Sort", heap_sort}
    };

    cout << "Algorithm\t\t1k\t\t10k\t\t50k\t\t100k\n";
    cout << "--------------------------------------------------------------------------------\n";

    for (auto& algo : algorithms) {
        cout << algo.first << "\t\t";
        for (int n : sizes) {
            vector<int> data = load_data("test_" + to_string(n) + ".txt");
            long long time = test_sort(algo.second, data);
            cout << time << "ms\t\t";
        }
        cout << "\n";
    }

    return 0;
}