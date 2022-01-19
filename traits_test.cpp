// C++ program to illustrate 
// std::is_trivially_destructible 
#include <bits/stdc++.h> 
#include <type_traits> 
using namespace std; 
  
// Declare structures 
struct Y { 
    // Constructor 
    Y(int, int){}; 
}; 
  
struct X { 
  
    // Destructor 
    ~X() noexcept(false) 
    { 
    } 
}; 
  
struct Z { 
    ~Z() = default; 
}; 
  
// Declare classes 
class A { 
    virtual void fn() {} 
}; 
  
// Driver Code 
int main() 
{ 
  
    cout << boolalpha; 
  
    // Check if int is trivially 
    // destructable or not 
    cout << "int:"
         << is_trivially_destructible<int>::value 
         << endl; 
  
    // Check if struct X is trivially 
    // destructable or not 
    cout << "struct X:"
         << is_trivially_destructible<X>::value 
         << endl; 
  
    // Check if struct Y is trivially 
    // destructable or not 
    cout << "struct Y:"
         << is_trivially_destructible<Y>::value 
         << endl; 
  
    // Check if struct Z is trivially 
    // destructable or not 
    cout << "struct Z:"
         << is_trivially_destructible<Z>::value 
         << endl; 
  
    // Check if class A is trivially 
    // destructable or not 
    cout << "class A:"
         << is_trivially_destructible<A>::value 
         << endl; 
  
    // Check if constructor Y(int, int) is 
    // trivially destructable or not 
    cout << "Constructor Y(int, int):"
         << is_trivially_destructible<Y(int, int)>::value 
         << endl; 
  
    bool bval = true;
    cout << bval << endl;
    return 0; 
}