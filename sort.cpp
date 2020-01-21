/*
 * 3_2. Сортировка почти упорядоченной последовательности:
 * Дана последовательность целых чисел a1...an и натуральное число k, такое что для любых i, j: если j >= i + k,
 * то a[i] <= a[j]. Требуется отсортировать последовательность. Последовательность может быть очень длинной.
 * Время работы O(n * log(k)). Доп. память O(k). Использовать слияние.
 */

#include <iostream>

template <class T>
void merge(T* array, int left_bound, int split, int right_bound) {
    int i = left_bound;
    int j = split + 1;
    int l = 0;

    int* buffer = new int[right_bound - left_bound + 1];

    while (i <= split && j <= right_bound)
        buffer[l++] = array[i] < array[j] ? array[i++] : array[j++];

    while (i <= split) buffer[l++] = array[i++];

    while (j <= right_bound) buffer[l++] = array[j++];

    for (l = 0; l < right_bound - left_bound + 1; ++l)
        array[left_bound + l] = buffer[l];
    delete[] buffer;
}

template <class T>
void merge_sort(T* array, int left_bound, int right_bound) {
    if (left_bound >= right_bound) return;
    int mid = (left_bound + right_bound) / 2;
    merge_sort(array, left_bound, mid);
    merge_sort(array, mid + 1, right_bound);
    merge(array, left_bound, mid, right_bound);
}


int main() {
    int numbers_count = 0;
    int k = 0;
    std::cin >> numbers_count >> k;

    int* array = new int[numbers_count];
    for (int i = 0; i < numbers_count; i++) {
        std::cin >> array[i];
    }

    for (int i = 0; i < numbers_count; i += k) {
        merge_sort(array, i, std::min(i + 2 * k, numbers_count - 1));
    }
    for (int i = 0; i < numbers_count; i++) {
        std::cout << array[i] << ' ';
    }
    std::cout << std::endl;
    return 0;
}
