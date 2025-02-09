# StudentVault

This project is a simple student database management system implemented in C++ using binary file I/O. It provides functionality for adding, listing, modifying, and deleting student records stored in a binary file.

## Features

- **Add Records:** Input new student records including first name, last name, course, and section.
- **List Records:** Display all student records in a formatted table.
- **Modify Records:** Search for a student record by last name and update its details.
- **Delete Records:** Remove a student record by searching via last name.
- **Cross-platform Compatibility:** Uses conditional system calls to clear the screen on both Windows and Unix-based systems.

## Project Structure

- `main.cpp` — Contains the source code for the Student Database Management System.
- `users.dat` — Binary file used to store student records (created automatically when the program is run).

## Getting Started

### Prerequisites

- A C++ compiler (such as `g++` or `clang++`).
- Basic knowledge of compiling and running C++ programs from the command line.

## Usage

When you run the program, a menu will be displayed with the following options:

- **Add Records:**  
  Enter new student details (first name, last name, course, and section) to add a record to the database.

- **List Records:**  
  View all student records stored in the system.

- **Modify Records:**  
  Search for a record by entering the student's last name. Once found, update the student’s details.

- **Delete Records:**  
  Remove a record by searching for a student's last name. The record will be deleted from the database.

- **Exit Program:**  
  Terminate the program.

Follow the on-screen prompts to interact with the database.
