#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <vector>

using namespace std;

template <typename Key, typename Value>
class HashTable {
private:
    struct Node {
        Key key;
        Value value;
        Node* next;

        Node(const Key& k, const Value& v) : key(k), value(v), next(nullptr) {}
    };

    Node** table;
    int capacity;
    int size;
    static constexpr double LOAD_FACTOR_THRESHOLD = 0.75;

    int hash(const Key& key) const {
        return key % capacity;
    }

    void rehash() {
        int oldCapacity = capacity;
        Node** oldTable = table;

        capacity *= 2;
        size = 0;
        table = new Node * [capacity]();

        for (int i = 0; i < capacity; ++i) {
            table[i] = nullptr;
        }

        for (int i = 0; i < oldCapacity; ++i) {
            Node* current = oldTable[i];
            while (current) {
                insert(current->key, current->value);
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }

        delete[] oldTable;
    }



public:
    HashTable() : size(0), capacity(16), table(new Node* [16]()) {
        for (int i = 0; i < capacity; ++i) {
            table[i] = nullptr;
        }
    }

    HashTable(int initialCapacity) : size(0), capacity(initialCapacity) {
        table = new Node * [capacity]();
        for (int i = 0; i < capacity; ++i) {
            table[i] = nullptr;
        }
    }

    HashTable(int initialCapacity, int count) : size(0), capacity(initialCapacity) {
        table = new Node * [capacity]();
        for (int i = 0; i < capacity; ++i) {
            table[i] = nullptr;
        }
        srand(static_cast<unsigned>(time(0)));
        for (int i = 0; i < count; ++i) {
            Key key = rand() % 1000;
            Value value = rand() % 100;
            insert(key, value);
        }
    }

    HashTable(const HashTable& other) : size(0), capacity(other.capacity) {
        table = new Node * [capacity]();
        for (int i = 0; i < capacity; ++i) {
            table[i] = nullptr;
        }
        for (int i = 0; i < capacity; ++i) {
            table[i] = nullptr;
            Node* current = other.table[i];
            while (current) {
                insert(current->key, current->value);
                current = current->next;
            }
        }
    }

    ~HashTable() {
        for (int i = 0; i < capacity; ++i) {
            Node* current = table[i];
            while (current) {
                Node* next = current->next;
                delete current;
                current = next;
            }
            table[i] = nullptr;
        }
        size = 0;
        delete[] table;
    }

    HashTable& operator=(const HashTable& other) {
        if (this != &other) {

            capacity = other.capacity;
            size = 0;
            table = new Node * [capacity]();
            for (int i = 0; i < capacity; ++i) {
                table[i] = nullptr;
                Node* current = other.table[i];
                while (current) {
                    insert(current->key, current->value);
                    current = current->next;
                }
            }
        }
        return *this;
    }

    bool insert(const Key& key, const Value& value) {
        //if ((double)(size + 1) / capacity > LOAD_FACTOR_THRESHOLD) {
        //    rehash();
        //}

        int index = hash(key);
        Node* current = table[index];
        
            while (current) {
                if (current->key == key) {
                    return false;
                }
                current = current->next;
            
        }
        Node* newNode = new Node(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        ++size;
        return true;
    }

    void insertOrAssign(const Key& key, const Value& value) {
        //if ((double)(size + 1) / capacity > LOAD_FACTOR_THRESHOLD) {
        //    rehash();
        //}

        int index = hash(key);
        Node* current = table[index];
        while (current) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }

        Node* newNode = new Node(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        ++size;
    }

    bool erase(const Key& key) {
        int index = hash(key);
        Node* current = table[index];
        Node* prev = nullptr;
        while (current) {
            if (current->key == key) {
                if (prev) {
                    prev->next = current->next;
                }
                else {
                    table[index] = current->next;
                }
                delete current;
                --size;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    Value* search(const Key& key) {
        int index = hash(key);
        Node* current = table[index];
        while (current) {
            if (current->key == key) {
                return &current->value;
            }
            current = current->next;
        }
        return nullptr;
    }

    bool contains(const Value& value) const {
        for (int i = 0; i < capacity; ++i) {
            Node* current = table[i];
            while (current) {
                if (current->value == value) {
                    return true;
                }
                current = current->next;
            }
        }
        return false;
    }

    int count(const Key& key) const {
        int index = hash(key);              
        int count = 0;                      
        Node* current = table[index];       
        while (current) {                  
            ++count;                        
            current = current->next;        
        }
        return count;                       
    }

    void print() const {
        for (int i = 0; i < capacity; ++i) {
            if (table[i]) {
                cout << "Bucket #" << i << ": ";
                for (Node* node = table[i]; node; node = node->next) {
                    cout << "[" << node->key << ", " << node->value << "] ";
                }
                cout << '\n';
            }
        }
    }

    int count_collisions() const {
        int counter = 0;
        for (int i = 0; i < size; i++)
        {
            if (table[i]) {
                Node* current = table[i];
                int cur_count = count(current->key);
                if (cur_count > 1)    counter += cur_count - 1;
            }

        }
        return counter;
    }


  
};

int main() {
    HashTable<int, int> hash_table1(3, 2);
    vector<int> num{ 25,30,35,40,45,50,55,60,75,125,175,225,275,325,375,425,475 };
    vector<float> collisions;
    for (int i = 0; i < num.size(); i++)
    {
        size_t table_size = num[i];
        float sum_median_collisions = 0;
        for (int j = 0; j < 100; j++)
        {
            HashTable<int, int> hash_table1(table_size, 25);
           /* hash_table1.print();*/
            float total_collisions = hash_table1.count_collisions();
            sum_median_collisions += (total_collisions / num[i]) * 100;
        }
        sum_median_collisions /= 100;
      /*  total_collisions = total_collisions / 100;*/

        collisions.push_back(sum_median_collisions);
    }
    for (int i = 0; i < collisions.size(); i++)
    {
        cout << collisions[i] << '\n';
    }
    return 0;
}

//int main() {
//    HashTable<int, int> map;
//    map.insert(1, 85);
//    map.insert(11, 86);
//    map.insert(2, 87);
//    map.insert(12, 88);
//
//    cout << "HashTable:\n";
//    map.print();
//
//    HashTable<int, int> map1(map);
//
//    cout << "\nCopied HashTable:\n";
//    map1.print();
//
//    map1.insert(3, 89);
//    map1.insert(33, 90);
//
//    cout << "\nCopied HashTable:\n";
//    map1.print();
//
//    map = map1;
//    cout << "\nHashTable:\n";
//    map.print();
//
//    map.insertOrAssign(23, 1);
//    map.insertOrAssign(33, 10);
//
//    cout << "\nHashTable:\n";
//    map.print();
//
//    map.insertOrAssign(23, 1000);
//
//    cout << "\nHashTable:\n";
//    map.print();
//
//    cout << "Is the table contains 85:" << map.contains(85) << "\n";
//
//    cout << "Value by index 23:" << *map.search(23);
//
//    map.erase(3);
//    map.insert(43, 12);
//    map.insert(53, 13);
//    map.insert(63, 14);
//
//    cout << "\nHashTable:\n";
//    map.print();
//    cout << map.count(63);
//
//
//
//    HashTable<char, int> romanNumbers;
//    romanNumbers.insert('I', 1);
//    romanNumbers.insert('V', 5);
//    romanNumbers.insert('X', 10);
//    romanNumbers.insert('L', 50);
//    romanNumbers.insert('C', 100);
//    romanNumbers.insert('D', 500);
//    romanNumbers.insert('M', 1000);
//    romanNumbers.print();
//
//    string input;
//    cout << "Enter romanial number ";
//    cin >> input;
//
//
//    int total = 0;
//    int prevValue = 0;
//
//    for (int i = input.length() - 1; i >= 0; i--) {
//        int currentValue = *romanNumbers.search(input[i]);
//        if (currentValue < prevValue) {
//            total -= currentValue;
//        }
//        else {
//            total += currentValue;
//        }
//        prevValue = currentValue;
//    }
//    cout << total;
//
//    return 0;
//}

