#include <iostream>
#include <vector>
#include <algorithm>
// using namespace std;

// 冒泡排序
// Tavg = O(n^2) Tbest = O(n) Twrost = O(n^2) S = O(1) In-place 稳定
void BubbleSort(std::vector<int> &nums){
    size_t n = nums.size();
    if(n <= 1) {
        return ;
    }
    bool is_swap = false;
    for(size_t i = 1; i < n; ++ i) {
        is_swap = false;
        for(size_t j = 1; j < n - i + 1; ++ j) {
            if(nums[j] < nums[j - 1]){
                std::swap(nums[j], nums[j - 1]);
                is_swap = true;
            }
        }
        if(is_swap == false){
            break ;
        }
    }
}


// 选择排序
// Tavg = O(n^2) Tbest = O(n^2) Twrost = O(n^2) S = O(1) In-sapce 不稳定
void SelectSort(std::vector<int> &nums){
    size_t n = nums.size();
    if(n <= 1)return ;
    size_t mid;
    for(size_t i = 0; i < n - 1; ++ i) {
        mid = i;
        for(size_t j = i + 1; j < n; ++ j){
            if(nums[j] < nums[mid]){
                mid = j;
            }
        } 
        std::swap(nums[mid], nums[i]);
    } 
}

// 插入排序
// Tavg = O(n^2) Tbest = O(n^2) Twrost = O(n^2) S = O(1) In-sapce 稳定
void InsertSort(std::vector<int> &nums){
    size_t n = nums.size();
    if(n <= 1)return ;
    for(size_t i = 0; i < n; ++ i){
        for(size_t j = i; j >= 1 && nums[j] < nums[j - 1]; -- j){
            std::swap(nums[j], nums[j - 1]);
        }
    }
}

// 希尔排序
// Tavg = O(nlogn) Tbest = O(nlog^2n) Twrost = O(nlog^2n) S = O(1) In-sapce 不稳定
void ShellSort(std::vector<int> &nums) {
    int n = nums.size();
    for(int gap = n / 2; gap > 0; gap /= 2) {
        for(int i = gap; i < n; ++ i) {
            for(int j = i; j - gap >= 0 && nums[j - gap] > nums[j]; j -= gap) {
                std::swap(nums[j - gap], nums[j]);
            }
        }
    }
}


// 归并数组
void Merge(std::vector<int> &nums, int l, int m, int r) {
    std::vector<int> tmp(r - l + 1);
    int i = l, j = m + 1, k = 0;
    while(i <= m && j <= r){
        if(nums[i] < nums[j]){
            tmp[k ++] = nums[i ++];
        }else {
            tmp[k ++] = nums[j ++];
        }
    }
    for(; i <= m; tmp[k ++] = nums[i ++]);
    for(; j <= r; tmp[k ++] = nums[j ++]);
    for(int k = 0; k <= (r - l); ++ k){
        nums[k + l] = tmp[k]; 
    }
}
// 归并排序
// Tavg = O(nlogn) Tbest = O(nlogn) Twrost = O(nlogn) S = O(n) Out-sapce 稳定
void MergeSort(std::vector<int> &nums, int l, int r) {
    // 递归终止条件
    // 将A[l...m]和A[m+1...r]合并为A[l,r]
    if(l >= r){
        return ;
    }
    int mid = l + (r - l) / 2;
    MergeSort(nums, l, mid);
    MergeSort(nums, mid+1, r);
    Merge(nums, l, mid, r);
}


// 快速排序
// Tavg = O(nlogn) Tbest = O(nlogn) Twrost = O(n^2) S = O(logn) In-sapce 不稳定
void QuickSort(std::vector<int> &nums, size_t l, size_t r) {
    if(l + 1 >= r) return;
    size_t first = l, last = r - 1;
    int key = nums[first];
    while(first < last) {
        while(first < last && nums[last] >= key) last --;
        nums[first] = nums[last];
        while(first < last && nums[first] <= key) first ++;
        nums[last] = nums[first];
    }
    nums[first] = key;
    QuickSort(nums, l, first);
    QuickSort(nums, first + 1, r);
}

// 堆化
void Heapify(std::vector<int> &nums, int n, int pos) {
    while(1) {
        int maxPos = pos;
        if(pos * 2 < n && nums[pos] < nums[pos * 2]) {
            maxPos = pos * 2;
        }
        if(pos * 2 + 1 < n && nums[maxPos] < nums[pos * 2 + 1]) {
            maxPos = pos * 2 + 1;
        }
        if(maxPos == pos) {
            break;
        }
        std::swap(nums[maxPos], nums[pos]);
        pos = maxPos;
    }
}
// 堆排序
// Tavg = O(nlogn) Tbest = O(nlogn) Twrost = O(nlogn) S = O(1) In-sapce 不稳定
void HeapSort(std::vector<int> &nums) {
    int n = nums.size();
    for(int i = n / 2; i >= 0; -- i) {
        Heapify(nums, n, i);
    }
    int k = n;
    while(k > 0){
        std::swap(nums[0], nums[-- k]);
        Heapify(nums, k, 0);
    }
}


// 计数排序
// Tavg = O(n+k) Tbest = O(n+k) Twrost = O(n+k) S = O(k) Out-sapce 稳定
void CountSort(std::vector<int> &nums) {
    if(nums.empty()) return ;
    int low = *std::min_element(nums.begin(), nums.end());
    int high = *std::max_element(nums.begin(), nums.end());
    int n = high - low + 1;
    std::vector<int> cnt(n);
    for(auto x : nums){
        cnt[x - low] ++;
    }
    for(int i = 0, pos = 0; i < n; ++ i){
        for(int j = 0; j < cnt[i]; ++ j){
            nums[pos ++] = i + low; 
        }
    }
}


// 桶排序
// Tavg = O(n+k) Tbest = O(n+k) Twrost = O(n^2) S = O(n+k) Out-sapce 稳定
void BucketSort(std::vector<int> &nums) {    
    // 桶长度50
    const int bucket_size = 50;
    int n = nums.size();
    int low = *std::min_element(nums.begin(), nums.end());
    int high = *std::max_element(nums.begin(), nums.end());
    int cnt = (high - low) / bucket_size + 1;
    std::vector<int> buckets[cnt];
    for(auto num : nums){
        buckets[(num - low) / bucket_size].push_back(num);
    }
    for(auto &bucket : buckets) {
        QuickSort(bucket, 0, bucket.size());
    }
    for(int i = 0, k = 0; k < n; ++ i){
        for(int j = 0; j < buckets[i].size(); ++ j){
            nums[k ++] = buckets[i][j];
        }
    }
}


// 基数排序
// Tavg = O(nxk) Tbest = O(nxk) Twrost = O(nxk) S = O(nxk) Out-sapce 稳定



void Print(std::vector<int> &nums) {
    for(auto num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}


int main(){
    std::vector<int> a = {34,66,2,5,95,4,46,27};
    // BubbleSort(a);
    // InsertSort(a);
    // SelectSort(a);
    // QuickSort(a, 0, a.size());
    // MergeSort(a, 0, a.size() - 1);
    // HeapSort(a);
    // BucketSort(a);
    // CountSort(a);
    ShellSort(a);
    Print(a);
    return 0;
}