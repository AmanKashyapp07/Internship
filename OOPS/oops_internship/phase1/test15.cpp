/**
 * File: test15.cpp
 * Demonstrates: Inheritance Basics (IS-A Relationship)
 * Description: Demonstrates deriving classes to inherit properties and methods from a base class.
 * Interview Takeaway: Inheritance establishes a class hierarchy, enabling code reuse and polymorphical pointers.
 */

#include <iostream>

class Buffer { // Class definition
public: // Accessible from anywhere in the program
    int* data;

    // Constructor: Allocates memory on the heap
    Buffer(int value) {
        data = new int(value); 
        std::cout << "Memory allocated at: " << data << "\n";
    }

    // Destructor: Deallocates memory
    ~Buffer() {
        std::cout << "Attempting to delete memory at: " << data << "\n";
        delete data; 
        std::cout << "Memory successfully deleted!\n";
    }
};

int main() { // Entry point of the program
    // 1. Create the first object
    Buffer b1(42); 

    {
        // 2. Create b2 as a copy of b1
        // Since we didn't write a copy constructor, C++ does a SHALLOW COPY.
        // b2.data now points to the exact same memory address as b1.data.
        Buffer b2 = b1; 
        
    } // 3. b2 goes out of scope here! 
      // b2's destructor runs and deletes the shared memory.

    std::cout << "Back in main scope...\n";

    return 0;
} // 4. b1 goes out of scope here!
  // b1's destructor runs and tries to delete the SAME memory again -> CRASH!