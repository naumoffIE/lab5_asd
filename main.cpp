#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

using namespace std;

template <typename K, typename T>
class hash_table {
private:
    struct Node {
        K key;
        T value;
        Node* next;

        Node(const K& key_, const T& value_) : key(key_), value(value_), next(nullptr) { };
    };

    Node** table;
    int capacity;
    int size;

    int hash(K key) const {
        return key % capacity;
    }

    void rehash() {
        int old_capacity = capacity;
        capacity *= 2;
        Node** new_table = new Node * [capacity];
        for (int i = 0; i < old_capacity; i++)
        {
            for (Node* current = table[i]; current; current = current->next)
            {
                new_table.insert(current);
            }
        }
    }


public:

    hash_table() : size(0), capacity(0), table(nullptr) { }
    // Конструктор по размеру
    hash_table(int initial_capacity) {
        capacity = initial_capacity;
        size = 0;
        table = new Node* [capacity];
        for (int i = 0; i < capacity; ++i)
            table[i] = nullptr;
    }

    // Конструктор со случайными значениями
    hash_table(int initial_capacity, int count) {
        capacity = initial_capacity;
        size = 0;
        table = new Node* [capacity];
        for (int i = 0; i < capacity; ++i)
            table[i] = nullptr;

        srand((unsigned)time(0));
        for (int i = 0; i < count; ++i) {
            K key = rand() % 1000;
            T value = rand() % 100;
            insert(key, value);
        }
    }

    // Конструктор копирования
    hash_table(const hash_table& other) {
        capacity = other.capacity;
        size = 0;
        table = new Node * [capacity];
        for (int i = 0; i < capacity; ++i){
            Node* current = other.table[i];
            while (current) {
                insert(current->key, current->value);
                current = current->next;
            }
        }
    }

    // Деструктор
    ~hash_table() {
            for (int i = 0; i < capacity; ++i) {
                Node* current = table[i];
                while (current) {
                    Node* next = current->next;
                    delete current;
                    current = next;
                }
                table[i] = nullptr;
            }
        delete[] table;
    }

    // Оператор присваивания
    hash_table& operator=(const hash_table& other) {
        if (this != &other) {
            clear_table();
            delete[] table;

            capacity = other.capacity;
            size = 0;
            table = new Node* [capacity];
            for (int i = 0; i < capacity; ++i)
                table[i] = nullptr;

            for (int i = 0; i < capacity; ++i) {
                Node* current = other.table[i];
                while (current) {
                    insert(current->key, current->value);
                    current = current->next;
                }
            }
        }
        return *this;
    }

    // Вставка
    bool insert(K key, const T& value) {
        int index = hash(key);
        Node* current = table[index];
        while (current) {
            if (current->key == key)
                return false;
            current = current->next;
        }
        Node* new_node = new Node( key, value );
        Node* head = table[index];
        new_node->next = head;
        table[index] = new_node;
        ++size;
        return true;
    }

    // Вставка или обновление
    void insert_or_assign(K key, T& value) {
        int index = hash(key);
        Node* current = table[index];
        while (current) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }
        Node* new_node = new Node{ key, value, table[index] };
        table[index] = new_node;
        ++size;
    }

    // Удаление
    bool erase(K key) {
        int index = hash(key);
        Node* current = table[index];
        Node* prev = nullptr;
        while (current) {
            if (current->key == key) {
                if (prev)
                    prev->next = current->next;
                else
                    table[index] = current->next;
                delete current;
                --size;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    // Поиск
    T* search(K key) {
        int index = hash(key);
        Node* current = table[index];
        while (current) {
            if (current->key == key)
                return &current->value;
            current = current->next;
        }
        return nullptr;
    }

    // Проверка наличия значения
    bool contains(T& value) const {
        for (int i = 0; i < capacity; ++i) {
            Node* current = table[i];
            while (current) {
                if (current->value == value)
                    return true;
                current = current->next;
            }
        }
        return false;
    }

    // Количество элементов по хешу
    int count(K key) {
        int index = hash(key);
        int cnt = 0;
        Node* current = table[index];
        while (current) {
            ++cnt;
            current = current->next;
        }
        return cnt;
    }

    // Печать таблицы
    void print() const {
        for (size_t i = 0; i < size; ++i) {
            cout << "Bucket #" << i << ": ";
            for (Node* node = table[i]; node != nullptr; node = node->next) {
                cout << "[" << node->key << ", " << node->value << "] ";
            }
            cout << '\n';
        }
    }
};


int main() {
    /*HashTable<int, int> map;
    map.insert(1, 85);
    map.insert(11, 86);
    map.insert(2, 87);
    map.insert(12, 88);

    cout << "HashTable:\n";
    map.print();

    HashTable<int, int> map1(map);

    cout << "\nCopied HashTable:\n";
    map1.print();

    map1.insert(3, 89);
    map1.insert(33, 90);

    cout << "\nCopied HashTable:\n";
    map1.print();

    map = map1;
    cout << "\nHashTable:\n";
    map.print();

    map.insert_or_assign(23, 1);
    map.insert_or_assign(33, 10);

    cout << "\nHashTable:\n";
    map.print();

    map.insert_or_assign(23, 1000);

    cout << "\nHashTable:\n";
    map.print();

    cout << "Is the table contains 85:" << map.contains(85) << "\n";

    cout << "Value by index 23:" << *map.search(23);

    map.erase(3);
    map.insert(43, 12);
    map.insert(53, 13);
    map.insert(63, 14);

    cout << "\nHashTable:\n";
    map.print();
    cout << map.count(63);*/



    hash_table<char, int> romanNumbers;
    romanNumbers.insert('I', 1);
    romanNumbers.insert('V', 5);
    romanNumbers.insert('X', 10);
    romanNumbers.insert('L', 50);
    romanNumbers.insert('C', 100);
    romanNumbers.insert('D', 500);
    romanNumbers.insert('M', 1000);
    romanNumbers.print();

    string input;
    cout << "Enter romanial number ";
    cin >> input;


    int total = 0;
    int prevValue = 0;

    for (int i = input.length() - 1; i >= 0; i--) {
        int currentValue = *romanNumbers.search(input[i]);
        if (currentValue < prevValue) {
            total -= currentValue;
        }
        else {
            total += currentValue;
        }
        prevValue = currentValue;
    }
    cout << total;

    return 0;
}
