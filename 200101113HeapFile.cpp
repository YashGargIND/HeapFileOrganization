#include<iostream>
#include<vector>
#define INT_SIZE 4
using namespace std;

class Record{
    public : 
        Record(int capacity, int primary_key){
            recordSize = capacity;
            primaryKey = primary_key;
        };
        virtual ~Record(){};
        int recordSize;
        int primaryKey;
};

class Page{
    public: 
        Page(int capacity){
            pageSize = capacity;
            prev = nullptr;
            next = nullptr;
            occupied = 4*INT_SIZE;
            recordCount = 0;

        };
        virtual ~Page(){};
        Page* prev;
        Page* next;
        bool isFull(){
            if(occupied>=pageSize){
                return true;
            }
            return false;
        };
        bool addRecord(int rec_size, int primary_key){
            if (isFull())
            {
                return false;
            }
            else{
                if(occupied + rec_size + INT_SIZE <= pageSize){
                    Record* R = new Record(rec_size, primary_key);
                    directory.push_back(R);
                    recordCount++;
                    occupied+=rec_size;
                    occupied+=INT_SIZE;
                    return true;
                }
                else{
                    return false;
                }
            }  
        };
        int searchPage(int key){
            for (int i = 0; i < recordCount; i++)
            {
                if (directory[i]->primaryKey==key)
                {
                    return i;
                }     
            }
            return -1;
            
        };
        int pageID;
        int recordCount;
        vector<Record*> directory;
    private: 
        int pageSize;
        int occupied;

};

class HeapFile{
    public:
        HeapFile(int page_capacity){
            pageCapacity = page_capacity;
            head = new Page(page_capacity);
            head->pageID = 0;
            pageCount = 1;
            tail = head;
        };
        virtual ~HeapFile(){};
        void insert(int rec_size, int primary_key){
            Page* cur = head;
            while(true){
                bool flag = cur->addRecord(rec_size, primary_key);
                if (flag == true)
                {
                    return;
                }
                else{
                    if(cur->next != nullptr){
                        cur = cur->next;
                        continue;
                    }
                    else{
                        addPageAtEnd();
                        tail->addRecord(rec_size, primary_key);
                        return;
                    }
                }
            }
        }
        pair<int, int> search(int key){ //returns PageID, Slot ID. {-1 , -1} if not exist
            Page* cur = head;
            while(true){
                int flag = cur->searchPage(key);
                if(flag != -1)
                {
                    return {cur->pageID , flag};
                }
                else{
                    if(cur->next != nullptr){
                        cur = cur->next;
                        continue;
                    }
                    else{
                        return {-1,-1};
                    }
                }
            }
        };
        void printStatus(){
            if(pageCount==1 & head->recordCount ==0){
                cout<<0<<endl;
                return;
            }
            cout<<pageCount<<" ";
            Page* cur = head;
            for (int i = 0; i < pageCount; i++)
            {
                cout<<cur->recordCount<<" ";
                cur = cur->next;
            }
            cout<<endl;
        }
    private:
        int pageCapacity;
        int pageCount;
        Page* head;
        Page* tail;
        Page* addPageAtEnd(){
            Page* new_page = new Page(pageCapacity);
            tail->next = new_page;
            new_page->prev = tail;
            new_page->pageID = tail->pageID + 1;
            tail = new_page;
            pageCount++;
            return new_page;
        };
};

int main()
{
    int page_size;

    cin >> page_size;

    HeapFile* file = new HeapFile(page_size);

    int n;
    int key,size;
    pair<int,int> result;

    while (cin >> n) {
        if (n == 1) {
            cin >> size >> key;
            file->insert(size, key);
        } else if (n == 2) {
            file->printStatus();
        } else if (n == 3) {
            cin >> key;
            result = file->search(key);
            cout << result.first << " " << result.second << endl;
        } else {
            return 0;
        }
    }
    return 0;
}