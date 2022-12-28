#include <iostream>
#include <stdexcept>

template<typename Key, typename Info>
class Dictionary {

private:
    
    template<typename NKey, typename NInfo>
    struct Node {
        NKey key;
        NInfo info;
        Node<NKey,NInfo> *next;
        
        Node(): key(), info(), next(nullptr) {};
        Node(const NKey &k, const NInfo &i): key(k), info(i), next(nullptr) {};
    };
    
    Node<Key,Info> *head, *tail;
    
    void printNode(const Node<Key,Info> *node, std::ostream &os) const;
    
public:

    //***** Default Constructor, Copy Constructor, Assignment Operator, Destructor *****
    Dictionary(): head(nullptr), tail(nullptr) {};
    Dictionary(const Dictionary<Key,Info> &dictionary): head(nullptr) { *this = dictionary; }
    Dictionary<Key,Info>& operator=(const Dictionary<Key,Info> &rhs);
    ~Dictionary();
    
    //***** Checking capacity and others *****
    void clear();
    bool isEmpty() const;
    int length() const;
    void print(std::ostream &os = std::cout) const;
    bool contains(const Key &key) const;
    
    //***** Operator overloading *****
    bool operator==(const Dictionary<Key,Info> &rhs) const;
    bool operator!=(const Dictionary<Key,Info> &rhs) const;
    Dictionary<Key,Info> operator+(const Dictionary<Key,Info> &rhs) const;
    Info& operator[](Key key);
    
    //***** Modifiers function *****
    bool insert(const Key &key, const Info &info);
    bool update(const Key &key, const Info &info);
    bool insertOrUpdate(const Key &key, const Info &info);
    bool erase(const Key &key);
    bool swap(Dictionary<Key,Info> &other);
    
};


template<typename Key, typename Info>
void Dictionary<Key,Info>::printNode(const Node<Key,Info> *node, std::ostream &os) const {
    os << "Key: " << node -> key << " Info: " << node -> info << std::endl;
}

//***** Default Constructor, Copy Constructor, Assignment Operator, Destructor *****

template<typename Key, typename Info>
Dictionary<Key,Info>& Dictionary<Key,Info>::operator=(const Dictionary<Key,Info> &rhs) {
    
    if(this == &rhs) {return *this;}
    
    this -> clear();
    
    Node<Key,Info> *temp;
    temp = rhs.head;
    while(temp != nullptr) {
        this -> insert(temp -> key, temp -> info);
        temp = temp -> next;
    }
    return *this;
}

template<typename Key, typename Info>
Dictionary<Key,Info>::~Dictionary() {
    clear();
} 

//************************************************************************************

//***** Operator Overloading *********************************************************

template<typename Key, typename Info>
bool Dictionary<Key,Info>::operator==(const Dictionary<Key,Info> &rhs) const {
    
    if(head == nullptr && rhs.head == nullptr) {return true;}
    if(length() != rhs.length()) {return false;}
    
    Node<Key,Info> *left = head;
    Node<Key,Info> *right = rhs.head;
    
    while(left != 0) {
        if(left -> key != right -> key || left -> info != right -> info) {return false;}
        
        left = left -> next;
        right = right -> next;
    }
    
    return true;
}

template<typename Key, typename Info>
bool Dictionary<Key,Info>::operator!=(const Dictionary<Key,Info> &rhs) const {
    return !(*this == rhs);
}

template<typename Key, typename Info>
Dictionary<Key,Info> Dictionary<Key,Info>::operator+(const Dictionary<Key,Info> &rhs) const {
    Dictionary<Key,Info> result;
    result = *this;
    auto temp = rhs.head;
    while(temp != nullptr) {
        if(contains(temp->key)) {
            result[temp->key] += temp -> info;  
        } else {
            result.insert(temp -> key, temp -> info);
        }
        temp = temp -> next;
    }
    return result;
}


template<typename Key, typename Info>
Info& Dictionary<Key,Info>::operator[](Key key) {
    
    if(!contains(key)) {
        throw std::logic_error("The key is not available in the map!");
    }
    
    Node<Key,Info> *temp = head;
    while(temp != nullptr) {
        if(temp -> key == key) {
            return temp -> info;
        }
        temp = temp -> next;
    }
    return temp -> info;
}
//***************************************************************************************

//***** Checking capacity and others ****************************************************
template<typename Key, typename Info>
void Dictionary<Key,Info>::clear() {
    
    while(head != nullptr) {
        tail = head;
        head = head -> next;
        delete tail;
    }
    tail = nullptr;
}

template<typename Key, typename Info>
bool Dictionary<Key,Info>::isEmpty() const {
    if(head == nullptr) {return true;}
    return false;
}

template<typename Key, typename Info>
int Dictionary<Key,Info>::length() const {
    
    int count {0};
    Node<Key,Info> *temp = head;
    while(temp != nullptr) {
        count++;
        temp = temp -> next;
    }
    return count;
}

template<typename Key, typename Info>
void Dictionary<Key,Info>::print(std::ostream &os) const {
    os << "Data in the dictionary: " << std::endl;
    auto temp = head;
    while(temp != nullptr) {
        printNode(temp, os);
        temp = temp -> next;
    }
    os << "===========================" <<  std::endl;
}

template<typename Key, typename Info>
bool Dictionary<Key,Info>::contains(const Key &key) const {
    
    auto temp = head;
    while(temp != nullptr) {
        if(temp -> key == key) {return true;}
        temp = temp -> next;
    }
    return false;
}
//************************************************************************************

//***** Modifiers function ***********************************************************
template<typename Key, typename Info>
bool Dictionary<Key,Info>::insert(const Key &key, const Info &info) {
    
    //Can't insert because the key is already in the map
    if(contains(key)) {
        return false;
    }
    
    Node<Key,Info> *newnode;
    newnode = new Node<Key,Info>(key, info);
    newnode -> next = nullptr;
    
    if(isEmpty()) {
        head = tail = newnode;
        return true;
    }
    
    tail -> next = newnode;
    tail = newnode;
    return true;
}

template<typename Key, typename Info>
bool Dictionary<Key,Info>::update(const Key &key, const Info &info) {
    
    auto temp = head;
    while(temp != nullptr) {
        if(temp -> key == key) {
            temp -> info = info;
            return true;
        }
        temp = temp -> next;
    }
    return false;
}

template<typename Key, typename Info>
bool Dictionary<Key,Info>::insertOrUpdate(const Key &key, const Info &info) {
    if(update(key,info)) {
        return true;
    } else {
        insert(key,info);
        return true;
    }
    return false;
}

template<typename Key, typename Info>
bool Dictionary<Key,Info>::erase(const Key &key) {
    
    //If the key is not found in the map
    if(!contains(key)) {
        return false;
    }
    
    Node<Key,Info>* temp = head;
    Node<Key,Info>* prev = nullptr;

    while(temp != nullptr) {
        if(temp -> key == key) {
            
            if(temp == head) { //If the key in the first node
                head = temp -> next;
                delete temp;
                return true;
            }
            
            prev -> next = temp -> next;
            if(temp == tail) {tail = prev;} //If the key in the last node
            delete temp;
            return true;
        }
        prev = temp;
        temp = temp -> next;
    }
    return false;
    
}

template<typename Key, typename Info>
bool Dictionary<Key,Info>::swap(Dictionary<Key,Info> &other) {
    
    if(this == &other) {return false;}
    
    Dictionary<Key,Info> temp;
    temp = *this;
    *this = other;
    other = temp;
    return true;
}

//************************************************************************************

template<typename Key, typename Info>
Dictionary<Key,Info> join(const Dictionary<Key,Info> &first, const Dictionary<Key,Info> &second) {
    Dictionary<Key,Info> result;
    result = first + second;
    return result;
}


//************************************************************************************

int main() {
    
    // Dictionary<int,int>a;
    // a.insert(2,3);
    // a.insert(1,3);
    // a.insert(3,4);
    // a.print();
    
    
    // a.insert(3,4);    
    // a.print();
    
    // a.update(3,5);
    // a.print(); 
    
    // a.insertOrUpdate(3,5);
    // a.insertOrUpdate(4,4);
    // a.print();
    
    // a.erase(2); //delete from the first node
    // a.print();
    // a.erase(3); //delete from the last node
    // a.print();
    // a.erase(1); //delete from the middle
    // a.print();
    
    // Dictionary<int,int>b1;
    // b1.insert(2,3);
    // b1.insert(1,3);
    // b1.insert(3,4);
    // Dictionary<int,int>b2;
    // b2.insert(8,6);
    // b2.insert(7,6);
    // b2.insert(9,6);
    // b2.insert(10,6);
    // b1.swap(b2);
    // b1.print();
    // b2.print();
    
    // a[3] = 5;
    // std::cout<< a[3] <<std::endl;
    
    Dictionary<int,int>b1;
    b1.insert(2,3);
    b1.insert(1,6);
    b1.insert(3,4);
    Dictionary<int,int>b2;
    b2.insert(2,6);
    b2.insert(1,6);
    b2.insert(3,6);
    b2.insert(10,6);
    Dictionary<int,int>result;
    result = join(b1,b2);
    result.print();
    
    // Dictionary<std::string,std::string>b1;
    // b1.insert("one","three ");
    // b1.insert("two","six ");
    // b1.insert("three","four ");
    // Dictionary<std::string,std::string>b2;
    // b2.insert("two","six");
    // b2.insert("one","six");
    // b2.insert("three","six");
    // b2.insert("ten","six");
    // Dictionary<std::string,std::string>result;
    // result = b1 + b2;
    // result.print();
    
    

}  
    
    
    