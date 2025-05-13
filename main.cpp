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
            clearTable();
            delete[] table;

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
        if ((size + 1) / capacity > LOAD_FACTOR_THRESHOLD) {
            rehash();
        }

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
        if ((size + 1) / capacity > LOAD_FACTOR_THRESHOLD) {
            rehash();
        }

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


  
};

int main() {
    vector<int> num{ 25,75,125,175,225,275,325,375,425,475 };
    vector<float> collisions;
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
        collisions.push_back(total_collisions);
    }
    for (int i = 0; i < collisions.size(); i++)
    {
        cout << collisions[i] << '\n';
    }
    return 0;
    return 0;
}
