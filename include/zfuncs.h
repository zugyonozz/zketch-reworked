#pragma once
#include <vector>
#include <functional>
#include <initializer_list>

class zFuncs {
public:
    // Default constructor
    zFuncs() = default;
    
    // Constructor with initializer list of functions
    zFuncs(std::initializer_list<std::function<void()>> funcList);
    
    // Copy constructor
    zFuncs(const zFuncs& other) = default;
    
    // Move constructor
    zFuncs(zFuncs&& other) noexcept = default;
    
    // Copy assignment
    zFuncs& operator=(const zFuncs& other) = default;
    
    // Move assignment
    zFuncs& operator=(zFuncs&& other) noexcept = default;
    
    // Destructor
    ~zFuncs() = default;
    
    // Add a single function (perfect forwarding)
    void add(std::function<void()> func);
    
    // Add a function with arguments (perfect forwarding with capture by value)
    template <typename Func, typename... Args>
    void add(Func&& func, Args&&... args) {
        funcs.emplace_back([func = std::forward<Func>(func), args...]() mutable { 
            func(args...); 
        });
    }
    
    // Add a function with arguments (capture by reference - use with caution)
    template <typename Func, typename... Args>
    void addRef(Func&& func, Args&... args) {
        funcs.emplace_back([&func, &args...]() { 
            func(args...); 
        });
    }
    
    // Execute all functions
    void operator()();
    
    // Execute all functions (explicit method)
    void executeAll();
    
    // Get number of functions
    size_t size() const noexcept;
    
    // Check if empty
    bool empty() const noexcept;
    
    // Execute a specific function by index (with bounds checking)
    void execute(size_t index);
    
    // Execute a specific function by index (safe version that returns bool)
    bool tryExecute(size_t index) noexcept;
    
    // Clear all functions
    void clear() noexcept;
    
    // Reserve space for functions
    void reserve(size_t capacity);
    
    // Remove function at specific index
    void remove(size_t index);
    
    // Check if index is valid
    bool isValidIndex(size_t index) const noexcept;

private:
    std::vector<std::function<void()>> funcs;
};