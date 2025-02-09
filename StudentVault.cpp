#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <cctype>

using namespace std;

struct Student {
    char firstName[50];
    char lastName[50];
    char course[100];
    int section;
};

// Cross-platform function to clear the screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Custom pause function that waits for the user to press Enter
void pauseScreen() {
    cout << "\n\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void addRecord(fstream &file) {
    char another = 'Y';
    Student s;
    
    while (toupper(another) == 'Y') {
        clearScreen();
        cout << "\n==============================================\n";
        cout << "         ADD NEW STUDENT RECORD\n";
        cout << "==============================================\n\n";

        // Flush any leftover input
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // First Name
        cout << "Enter First Name  : ";
        cin.getline(s.firstName, sizeof(s.firstName));

        // Last Name
        cout << "Enter Last Name   : ";
        cin.getline(s.lastName, sizeof(s.lastName));

        // Course
        cout << "Enter Course      : ";
        cin.getline(s.course, sizeof(s.course));

        // Section (numeric input validation)
        while (true) {
            cout << "Enter Section (numeric): ";
            if (cin >> s.section) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear remaining input
                break;
            } else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "   [Error] Invalid input. Please enter numbers only.\n";
            }
        }

        // Write record to the end of the file.
        file.clear();                  
        file.seekp(0, ios::end);      
        file.write(reinterpret_cast<char*>(&s), sizeof(s));
        if (!file) {
            cerr << "\n   [Error] Writing record to file failed!\n";
        } else {
            cout << "\n   >> Record added successfully!\n";
        }
        
        // Prompt to add another record.
        do {
            cout << "\nDo you want to add another record? (Y/N): ";
            cin.get(another);
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } while (toupper(another) != 'Y' && toupper(another) != 'N');
    }
}

void listRecords(fstream &file) {
    clearScreen();
    file.clear();
    file.seekg(0, ios::beg);
    Student s;
    
    cout << "\n==============================================\n";
    cout << "           LIST OF STUDENT RECORDS\n";
    cout << "==============================================\n\n";
    
    // Print table header
    cout << left << setw(15) << "First Name"
         << left << setw(15) << "Last Name"
         << left << setw(25) << "Course"
         << left << setw(10) << "Section" << "\n";
    cout << setfill('-') << setw(65) << "-" << setfill(' ') << "\n";

    bool recordsFound = false;
    while (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        cout << left << setw(15) << s.firstName
             << left << setw(15) << s.lastName
             << left << setw(25) << s.course
             << left << setw(10) << s.section << "\n";
        recordsFound = true;
    }
    if (!recordsFound)
        cout << "No records found!\n";
        
    cout << "\n" << setfill('=') << setw(65) << "=" << setfill(' ') << "\n";
    pauseScreen();
}

void modifyRecord(fstream &file) {
    char another = 'Y';
    char searchLastName[50];
    Student s;
    while (toupper(another) == 'Y') {
        clearScreen();
        cout << "\n==============================================\n";
        cout << "          MODIFY STUDENT RECORD\n";
        cout << "==============================================\n\n";
        
        cout << "Enter the Last Name of the student to modify: ";
        cin.getline(searchLastName, sizeof(searchLastName));
        
        bool found = false;
        file.clear();
        file.seekg(0, ios::beg);
        while (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
            if (strcmp(s.lastName, searchLastName) == 0) {
                cout << "\n----- Record Found -----\n";
                cout << left << setw(15) << "First Name:" << s.firstName << "\n";
                cout << left << setw(15) << "Last Name:"  << s.lastName << "\n";
                cout << left << setw(15) << "Course:"     << s.course << "\n";
                cout << left << setw(15) << "Section:"    << s.section << "\n";
                cout << "-------------------------\n\n";
                
                cout << "Enter New Details Below:\n";
                
                // New First Name
                cout << "New First Name  : ";
                cin.getline(s.firstName, sizeof(s.firstName));
                
                // New Last Name
                cout << "New Last Name   : ";
                cin.getline(s.lastName, sizeof(s.lastName));
                
                // New Course
                cout << "New Course      : ";
                cin.getline(s.course, sizeof(s.course));
                
                // New Section (with validation)
                while (true) {
                    cout << "New Section (numeric): ";
                    if (cin >> s.section) {
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    } else {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "   [Error] Invalid input. Please enter numbers only.\n";
                    }
                }
                
                // Calculate the position for overwriting the record.
                streampos pos = file.tellg();
                file.clear();
                file.seekp(pos - streamoff(sizeof(s)));
                file.write(reinterpret_cast<char*>(&s), sizeof(s));
                if (!file) {
                    cerr << "\n   [Error] Updating record in file failed!\n";
                } else {
                    cout << "\n   >> Record updated successfully!\n";
                }
                found = true;
                break;
            }
        }
        if (!found)
            cout << "\n   [Info] Record not found!\n";
            
        cout << "\nDo you want to modify another record? (Y/N): ";
        cin >> another;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void deleteRecord(fstream &file) {
    char another = 'Y';
    char searchLastName[50];
    Student s;
    while (toupper(another) == 'Y') {
        clearScreen();
        cout << "\n==============================================\n";
        cout << "          DELETE STUDENT RECORD\n";
        cout << "==============================================\n\n";
        
        cout << "Enter the Last Name of the student to delete: ";
        cin.getline(searchLastName, sizeof(searchLastName));
        
        // Open a temporary file for records that we want to keep.
        ofstream tempFile("temp.dat", ios::binary);
        if (!tempFile) {
            cerr << "\n   [Error] Unable to open temporary file!\n";
            return;
        }
        
        file.clear();
        file.seekg(0, ios::beg);
        bool found = false;
        while (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
            if (strcmp(s.lastName, searchLastName) != 0)
                tempFile.write(reinterpret_cast<char*>(&s), sizeof(s));
            else
                found = true;
        }
        tempFile.close();
        file.close();
        
        // Replace the old file with the temporary file.
        if (remove("users.dat") != 0) {
            cerr << "\n   [Error] Deleting original file failed!\n";
        }
        if (rename("temp.dat", "users.dat") != 0) {
            cerr << "\n   [Error] Renaming temporary file failed!\n";
        }
        
        // Reopen the main file.
        file.open("users.dat", ios::binary | ios::in | ios::out);
        if (!file) {
            cerr << "\n   [Error] Reopening file failed!\n";
            exit(1);
        }
        
        if (found)
            cout << "\n   >> Record deleted successfully!\n";
        else
            cout << "\n   [Info] No matching record found!\n";
            
        cout << "\nDo you want to delete another record? (Y/N): ";
        cin >> another;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    fstream file;
    // Option A: Open the file in a clean (empty) state.
    file.open("users.dat", ios::binary | ios::in | ios::out | ios::trunc);
    if (!file) {
        cerr << "\n[Error] Cannot open file!\n";
        return 1;
    }
    
    char choice;
    do {
        clearScreen();
        cout << "\n=====================================================\n";
        cout << "       STUDENT DATABASE MANAGEMENT SYSTEM\n";
        cout << "=====================================================\n";
        cout << "\n\t1. Add Records";
        cout << "\n\t2. List Records";
        cout << "\n\t3. Modify Records";
        cout << "\n\t4. Delete Records";
        cout << "\n\t5. Exit Program";
        cout << "\n\n=====================================================\n";
        cout << "Enter your choice (1-5): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case '1':
                addRecord(file);
                break;
            case '2':
                listRecords(file);
                break;
            case '3':
                modifyRecord(file);
                break;
            case '4':
                deleteRecord(file);
                break;
            case '5':
                cout << "\n   THANK YOU FOR USING THE STUDENT DATABASE SYSTEM.\n";
                break;
            default:
                cout << "\n   [Error] Invalid Choice! Please try again.\n";
                pauseScreen();
        }
    } while (choice != '5');
    
    file.close();
    return 0;
}
