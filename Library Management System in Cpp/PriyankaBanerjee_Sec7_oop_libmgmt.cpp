/*
Priyanka Banerjee- 2312200001271
Sarmistha Ghosh- 2312200001148
Simran Ghosh- 2312200001264
Ankita Bera- 2312200001322
Sriza Roy- 2312200001333
*/

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <algorithm>
using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    string genre;
    int publicationYear;
    bool isAvailable;

    Book(int id, string title, string author, string genre, int publicationYear)
        : id(id), title(title), author(author), genre(genre), publicationYear(publicationYear), isAvailable(true) {}
};

class Member {
public:
    int memberId;
    string name;
    string address;
    string contactInfo;
    vector<pair<int, time_t>> borrowedBooks; // Pair of book ID and due date.

    Member(int memberId, string name, string address, string contactInfo)
        : memberId(memberId), name(name), address(address), contactInfo(contactInfo) {}
};

class LibrarySystem {
    vector<Book> books;
    vector<Member> members;

    time_t calculateDueDate() {
        time_t now = time(0);
        return now + (14 * 24 * 60 * 60); // 14 days in seconds.
    }

    int calculateOverdueDays(time_t dueDate) {
        time_t now = time(0);
        return max(0, static_cast<int>((now - dueDate) / (24 * 60 * 60)));
    }

public:
    void addBook() {
        int id, publicationYear;
        string title, author, genre;
        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        cout << "Enter Genre: ";
        getline(cin, genre);
        cout << "Enter Publication Year: ";
        cin >> publicationYear;

        books.emplace_back(id, title, author, genre, publicationYear);
        cout << "Book added successfully.\n";
    }

    void removeBook() {
        int id;
        cout << "Enter Book ID to remove: ";
        cin >> id;

        auto it = remove_if(books.begin(), books.end(), [&](const Book& book) { return book.id == id; });
        if (it != books.end()) {
            books.erase(it, books.end());
            cout << "Book removed successfully.\n";
        } else {
            cout << "Book not found.\n";
        }
    }

    void registerMember() {
        int memberId;
        string name, address, contactInfo;
        cout << "Enter Member ID: ";
        cin >> memberId;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Address: ";
        getline(cin, address);
        cout << "Enter Contact Info: ";
        getline(cin, contactInfo);

        members.emplace_back(memberId, name, address, contactInfo);
        cout << "Member registered successfully.\n";
    }

    void removeMember() {
        int memberId;
        cout << "Enter Member ID to remove: ";
        cin >> memberId;

        auto it = remove_if(members.begin(), members.end(), [&](const Member& member) { return member.memberId == memberId; });
        if (it != members.end()) {
            members.erase(it, members.end());
            cout << "Member removed successfully.\n";
        } else {
            cout << "Member not found.\n";
        }
    }

    void borrowBook(int memberId, int bookId) {
        auto memberIt = find_if(members.begin(), members.end(), [&](const Member& member) { return member.memberId == memberId; });
        if (memberIt == members.end()) {
            cout << "Member not found.\n";
            return;
        }

        auto bookIt = find_if(books.begin(), books.end(), [&](const Book& book) { return book.id == bookId; });
        if (bookIt == books.end() || !bookIt->isAvailable) {
            cout << "Book not available.\n";
            return;
        }

        bookIt->isAvailable = false;
        memberIt->borrowedBooks.emplace_back(bookId, calculateDueDate());
        cout << "Book borrowed successfully.\n";
    }

    void returnBook(int memberId, int bookId) {
        auto memberIt = find_if(members.begin(), members.end(), [&](const Member& member) { return member.memberId == memberId; });
        if (memberIt == members.end()) {
            cout << "Member not found.\n";
            return;
        }

        auto& borrowedBooks = memberIt->borrowedBooks;
        auto it = find_if(borrowedBooks.begin(), borrowedBooks.end(), [&](const pair<int, time_t>& p) { return p.first == bookId; });
        if (it == borrowedBooks.end()) {
            cout << "Book not borrowed by this member.\n";
            return;
        }

        int overdueDays = calculateOverdueDays(it->second);
        if (overdueDays > 0) {
            cout << "Overdue fine: Rs " << overdueDays * 5 << "\n";
        }

        borrowedBooks.erase(it);
        auto bookIt = find_if(books.begin(), books.end(), [&](const Book& book) { return book.id == bookId; });
        if (bookIt != books.end()) {
            bookIt->isAvailable = true;
        }
        cout << "Book returned successfully.\n";
    }

    void generateReport() {
        cout << "Borrowed Books Report:\n";
        for (const auto& member : members) {
            for (const auto& borrowed : member.borrowedBooks) {
                cout << "Member ID: " << member.memberId << ", Book ID: " << borrowed.first << ", Due Date: "
                     << ctime(&borrowed.second);
            }
        }
    }

    void showOverdueList() {
        cout << "Overdue List:\n";
        for (const auto& member : members) {
            for (const auto& borrowed : member.borrowedBooks) {
                int overdueDays = calculateOverdueDays(borrowed.second);
                if (overdueDays > 0) {
                    cout << "Member ID: " << member.memberId << ", Book ID: " << borrowed.first
                         << ", Overdue Days: " << overdueDays << ", Fine: Rs " << overdueDays * 5 << "\n";
                }
            }
        }
    }
};

int main() {
    LibrarySystem library;
    int choice, memberId, bookId;

    do {
        cout << "\nLibrary Management System Menu:\n"
             << "1. Add Book\n2. Remove Book\n3. Register Member\n4. Remove Member\n"
             << "5. Borrow Book\n6. Return Book\n7. Generate Report\n8. Show Overdue List\n0. Exit\n"
             << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                library.addBook();
                break;
            case 2:
                library.removeBook();
                break;
            case 3:
                library.registerMember();
                break;
            case 4:
                library.removeMember();
                break;
            case 5:
                cout << "Enter Member ID and Book ID: ";
                cin >> memberId >> bookId;
                library.borrowBook(memberId, bookId);
                break;
            case 6:
                cout << "Enter Member ID and Book ID: ";
                cin >> memberId >> bookId;
                library.returnBook(memberId, bookId);
                break;
            case 7:
                library.generateReport();
                break;
            case 8:
                library.showOverdueList();
                break;
            case 0:
                cout << "Exiting system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
