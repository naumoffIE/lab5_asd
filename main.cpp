#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <string>

template <typename K, typename T>
class HashTable {
private:
    std::vector<std::list<std::pair<K, T>>> _table;
    int _size;

    int hash(const K& key) const {
        return (key) % _size;
    }
    double load_factor_threshold = 0.75;

public:
    // Пустой конструктор
    HashTable(int size) : _size(size), _table(size) {}

    // Конструктор со случайными значениями
    HashTable(int size, int count) : _size(size), _table(size) {
        srand(time(0));
        for (int i = 0; i < count; ++i) {
            K key = rand() % 1000;
            T value = rand() % 100;
            insert(key, value);
        }
    }

    // Конструктор копирования
    HashTable(const HashTable& other) : _size(other._size), _table(other._table) {}

    // Деструктор
    ~HashTable() = default;

    // Оператор присваивания
    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            _size = other._size;
            _table = other._table;
        }
        return *this;
    }

    // Печать содержимого
    void print() const {
        for (int i = 0; i < _size; ++i) {
            std::cout << i << ": ";
            for (const auto& pair : _table[i]) {
                std::cout << "[" << pair.first << ": " << pair.second << "] ";
            }
            std::cout << "\n";
        }
    }

    // Вставка
    bool insert(K key, const T& value) {
        int idx = hash(key);
        for (auto& pair : _table[idx]) {
            if (pair.first == key)
                return false;
        }
        _table[idx].emplace_back(key, value);
        return true;
    }

    // Вставка или присваивание
    void insert_or_assign(K key, T& value) {
        int idx = hash(key);
        for (auto& pair : _table[idx]) {
            if (pair.first == key) {
                pair.second = value;
                return;
            }
        }
        _table[idx].emplace_back(key, value);
    }

    // Проверка наличия значения
    bool contains(T& value) const {
        for (const auto& bucket : _table) {
            for (const auto& pair : bucket) {
                if (pair.second == value)
                    return true;
            }
        }
        return false;
    }

    // Поиск по ключу
    T* search(K key) {
        int idx = hash(key);
        for (auto& pair : _table[idx]) {
            if (pair.first == key)
                return &pair.second;
        }
        return nullptr;
    }

    // Удаление по ключу
    bool erase(K key) {
        int idx = hash(key);
        auto& bucket = _table[idx];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->first == key) {
                bucket.erase(it);
                return true;
            }
        }
        return false;
    }

    // Кол-во элементов с таким же значением хеша
    int count(K key) {
        int idx = hash(key);
        return _table[idx]._size();
    }

    bool is_busy(size_t index)
    {
        if (table[index].element != nullptr)
        {
            return table[index].element->_status;
        }
        return false;
    }
};


int main() {
    HashTable<char, std::string> map(10);
    map.insert(1, "apple");
    map.insert(2, "banana");
    map.insert_or_assign(1, std::string("apricot"));

    map.print();


    
    std::vector<int> num{ 25,75,125,175,225,275,325,375,425,475 };
    std::vector<float> coliz;
    for (int i = 0; i < num.size(); i++)
    {
        size_t table_size = num[i];
        float total_collisions = 0;
        for (int i = 0; i < 100; i++)
        {
            HashTable<int, int> hash_table1(table_size, 25);
            total_collisions += hash_table1.count_hashtable();
        }
        total_collisions = total_collisions / 100;
        coliz.push_back(total_collisions);
    }
    for (int i = 0; i < coliz.size(); i++)
    {
        std::cout << coliz[i] << '\n';
    }
    return 0;
}
