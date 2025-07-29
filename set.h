#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Set
{
    private:
        vector<int> elements;
    public:
        Set();  
        Set(const vector<int>& arr);  
        int size() const;
        bool isEmpty() const;
        bool isIn(int element) const;
        void add(int element);
        Set intersection(const Set& second) const;
        Set unionSet(const Set& second) const;
        Set difference(const Set& second) const;
        bool isSubset(const Set& second) const;
        bool isProperSubset(const Set& second) const;
        
        friend ostream& operator<<(ostream& out, const Set& set);
        friend bool operator^(const Set& set, int element);
        Set& operator+=(int element);       
        Set operator*(const Set& second) const;  
        Set operator+(const Set& second) const;  
        Set operator-(const Set& second) const;  
        bool operator<(const Set& second) const; 
        bool operator<=(const Set& second) const; 
        bool operator!() const; 
};
