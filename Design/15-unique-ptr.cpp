#include <iostream>
#include <utility>
#include <cstddef>
using namespace std;

/**
 * ============================================================================
 * DESIGN 15: SMART POINTER (std::unique_ptr Clone)
 * ============================================================================
 * 
 * A Unique Pointer is a scoped smart pointer that owns and manages another
 * object through a pointer and disposes of that object when the unique_ptr goes
 * out of scope (RAII - Resource Acquisition Is Initialization).
 * 
 * Key Interview Concepts:
 * 1. Exclusive Ownership: Exactly ONE unique_ptr owns the underlying resource.
 * 2. Copy Operations are DELETED (`= delete`) because copying violates exclusive ownership.
 * 3. Move Semantics are SUPPORTED (Move Constructor & Move Assignment transfer ownership).
 * 4. Core Methods: get(), release(), reset(), operator*(), operator->(), operator bool().
 * 5. Array Specialization `UniquePtr<T[]>` with `delete[]`.
 * 6. `make_unique` with Perfect Forwarding (`std::forward<Args>`).
 */

// ============================================================================
// 1. PRIMARY TEMPLATE: SINGLE OBJECT UNIQUE_PTR<T>
// ============================================================================
template <typename T>
class UniquePtr {
private:
    T* ptr;

public:
    // Default and Raw Pointer Constructor
    explicit UniquePtr(T* p = nullptr) noexcept : ptr(p) {}

    // Destructor: Automatically frees managed heap memory (RAII)
    ~UniquePtr() {
        delete ptr;
    }

    // ------------------------------------------------------------------------
    // DISABLE COPYING (Exclusive Ownership)
    // ------------------------------------------------------------------------
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // ------------------------------------------------------------------------
    // MOVE SEMANTICS (Transfer Ownership)
    // ------------------------------------------------------------------------
    // Move Constructor
    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    // Move Assignment Operator
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;          // Free currently owned resource
            ptr = other.ptr;     // Take ownership of other's resource
            other.ptr = nullptr; // Nullify source pointer
        }
        return *this;
    }

    // ------------------------------------------------------------------------
    // DEREFERENCING & ACCESS OPERATORS
    // ------------------------------------------------------------------------
    T& operator*() const {
        return *ptr;
    }

    T* operator->() const noexcept {
        return ptr;
    }

    T* get() const noexcept {
        return ptr;
    }

    // Boolean context check: if (myPtr) { ... }
    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }

    // ------------------------------------------------------------------------
    // MODIFIERS
    // ------------------------------------------------------------------------

    // Relinquishes ownership of the managed object and returns the raw pointer
    T* release() noexcept {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    // Replaces the managed object with a new raw pointer (default nullptr)
    void reset(T* p = nullptr) noexcept {
        if (ptr != p) {
            delete ptr;
            ptr = p;
        }
    }

    // Swaps ownership with another UniquePtr
    void swap(UniquePtr& other) noexcept {
        std::swap(ptr, other.ptr);
    }
};

// ============================================================================
// 2. TEMPLATE SPECIALIZATION: DYNAMIC ARRAYS UNIQUE_PTR<T[]>
// ============================================================================
template <typename T>
class UniquePtr<T[]> {
private:
    T* ptr;

public:
    explicit UniquePtr(T* p = nullptr) noexcept : ptr(p) {}

    // Array Destructor uses delete[]
    ~UniquePtr() {
        delete[] ptr;
    }

    // Disable Copying
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Move Semantics
    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete[] ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // Array Subscript Operator
    T& operator[](size_t index) const {
        return ptr[index];
    }

    T* get() const noexcept { return ptr; }

    explicit operator bool() const noexcept { return ptr != nullptr; }

    T* release() noexcept {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    void reset(T* p = nullptr) noexcept {
        if (ptr != p) {
            delete[] ptr;
            ptr = p;
        }
    }
};

// ============================================================================
// 3. MAKE_UNIQUE HELPER WITH PERFECT FORWARDING (C++14 std::make_unique clone)
// ============================================================================
template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}
