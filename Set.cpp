#include "set.h"

Set::Set() 
{
}

Set::Set(const vector<int>& vec) 
{
    for (int element : vec) 
        add(element);
}

int Set::size() const 
{
    return elements.size();
}

bool Set::isEmpty() const 
{
    return elements.empty();
}

bool Set::isIn(int element) const 
{
    bool result = find(elements.begin(), elements.end(), element) != elements.end();
    return result;
}

void Set::add(int element) 
{
    if (!isIn(element)) 
    {
        elements.push_back(element);
        sort(elements.begin(), elements.end());  
    }
}

Set Set::intersection(const Set& second) const 
{
    Set result;
    for (int element : elements) 
        if (second.isIn(element)) 
            result.add(element);

    return result;
}

Set Set::unionSet(const Set& second) const 
{
    Set result = *this; // To access the the element of the set calling this function
    for (int element : second.elements) 
        result.add(element);

    return result;
}

Set Set::difference(const Set& second) const 
{
    Set result;
    for (int element : elements) 
        if (!second.isIn(element)) 
            result.add(element);

    return result;
}

bool Set::isSubset(const Set& second) const 
{
    for (int element : elements) 
        if (!second.isIn(element)) 
            return false;

    return true;
}

bool Set::isProperSubset(const Set& second) const 
{
    bool result = size() < second.size() && isSubset(second);
    return result;
}

ostream& operator<<(ostream& out, const Set& set) 
{
    out << "{";
    for (size_t i = 0; i < set.elements.size(); ++i) 
    {
        out << set.elements[i];
        if (i < set.elements.size() - 1) 
            out << ", ";
    }
    out << "}";

    return out;
}

bool operator^(const Set& set, int element) 
{
    return set.isIn(element);
}

Set& Set::operator+=(int element) 
{
    add(element);

    return *this; // To be able to chain += operation
}

Set Set::operator*(const Set& second) const 
{
    return intersection(second);
}

Set Set::operator+(const Set& second) const 
{
    return unionSet(second);
}

Set Set::operator-(const Set& second) const 
{
    return difference(second);
}

bool Set::operator<(const Set& second) const 
{
    return isProperSubset(second);
}

bool Set::operator<=(const Set& second) const 
{
    return isSubset(second);
}

bool Set::operator!() const 
{
    return isEmpty();
}
