template<typename T> 
class unique_ptr  { 
   T* p; 
public: 
    unique_ptr(T* s = nullptr) :p(s) {} 
    ~unique_ptr() { delete p; } 
  
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;
 
    unique_ptr(unique_ptr&& s) :p(s.p) { 
        s.p = nullptr;
    } 
 
    unique_ptr& operator=(unique_ptr s) { 
        delete p; 
        p = s.p; 
        s.p=nullptr; 
        return *this; 
    } 
 
    T* operator->() const { return p; } 
    T& operator*() const { return *p; } 
}; 