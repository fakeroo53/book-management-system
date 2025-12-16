#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BOOKS 100
#define MAX_TITLE_LEN 100
#define MAX_AUTHOR_LEN 100
#define MAX_ISBN_LEN 20

typedef struct {
    int id;
    char title[MAX_TITLE_LEN];
    char author[MAX_AUTHOR_LEN];
    char isbn[MAX_ISBN_LEN];
    int year;
    float price;
    int quantity;
} Book;

typedef struct {
    Book books[MAX_BOOKS];
    int count;
} Library;

Library library = {0};

// Function prototypes
void displayMenu();
void addBook();
void viewAllBooks();
void searchBook();
void updateBook();
void deleteBook();
void viewBookStatistics();
void saveToFile();
void loadFromFile();
void clearInputBuffer();
int getBookIndexById(int id);

// Helper function to clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Display main menu
void displayMenu() {
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║      BOOK MANAGEMENT SYSTEM MENU       ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("1. Add a New Book\n");
    printf("2. View All Books\n");
    printf("3. Search Book by Title or Author\n");
    printf("4. Update Book Information\n");
    printf("5. Delete a Book\n");
    printf("6. View Library Statistics\n");
    printf("7. Exit\n");
    printf("─────────────────────────────────────────\n");
    printf("Enter your choice (1-7): ");
}

// Add a new book to the library
void addBook() {
    if (library.count >= MAX_BOOKS) {
        printf("\n⚠ Library is full! Cannot add more books.\n");
        return;
    }

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║          ADD A NEW BOOK                ║\n");
    printf("╚════════════════════════════════════════╝\n");

    Book newBook;
    newBook.id = library.count + 1;

    printf("Enter Book Title: ");
    fgets(newBook.title, MAX_TITLE_LEN, stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0;

    if (strlen(newBook.title) == 0) {
        printf("❌ Title cannot be empty!\n");
        return;
    }

    printf("Enter Author Name: ");
    fgets(newBook.author, MAX_AUTHOR_LEN, stdin);
    newBook.author[strcspn(newBook.author, "\n")] = 0;

    if (strlen(newBook.author) == 0) {
        printf("❌ Author name cannot be empty!\n");
        return;
    }

    printf("Enter ISBN: ");
    fgets(newBook.isbn, MAX_ISBN_LEN, stdin);
    newBook.isbn[strcspn(newBook.isbn, "\n")] = 0;

    printf("Enter Publication Year: ");
    if (scanf("%d", &newBook.year) != 1) {
        printf("❌ Invalid year input!\n");
        clearInputBuffer();
        return;
    }

    printf("Enter Price ($): ");
    if (scanf("%f", &newBook.price) != 1) {
        printf("❌ Invalid price input!\n");
        clearInputBuffer();
        return;
    }

    printf("Enter Quantity: ");
    if (scanf("%d", &newBook.quantity) != 1) {
        printf("❌ Invalid quantity input!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    library.books[library.count] = newBook;
    library.count++;

    printf("\n✅ Book added successfully! (Book ID: %d)\n", newBook.id);
}

// View all books in the library
void viewAllBooks() {
    if (library.count == 0) {
        printf("\n📚 The library is empty. No books to display.\n");
        return;
    }

    printf("\n╔════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                              ALL BOOKS IN LIBRARY                                 ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("| ID | Title                    | Author              | ISBN         | Year | Price |\n");
    printf("├────┼──────────────────────────┼─────────────────────┼───────────────┼──────┼───────┤\n");

    for (int i = 0; i < library.count; i++) {
        Book *book = &library.books[i];
        printf("| %2d | %-24s | %-19s | %-13s | %4d | $%-5.2f |\n",
               book->id,
               book->title,
               book->author,
               book->isbn,
               book->year,
               book->price);
    }

    printf("├────┴──────────────────────────┴─────────────────────┴───────────────┴──────┴───────┤\n");
    printf("| Total Books: %-79d |\n", library.count);
    printf("╚════════════════════════════════════════════════════════════════════════════════════╝\n");
}

// Get book index by ID
int getBookIndexById(int id) {
    for (int i = 0; i < library.count; i++) {
        if (library.books[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Search for a book
void searchBook() {
    if (library.count == 0) {
        printf("\n📚 The library is empty. No books to search.\n");
        return;
    }

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║          SEARCH FOR A BOOK             ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("Search by:\n");
    printf("1. Title\n");
    printf("2. Author\n");
    printf("Enter choice (1-2): ");

    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("❌ Invalid input!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (choice < 1 || choice > 2) {
        printf("❌ Invalid choice!\n");
        return;
    }

    char searchTerm[100];
    int found = 0;

    if (choice == 1) {
        printf("Enter Book Title: ");
        fgets(searchTerm, 100, stdin);
        searchTerm[strcspn(searchTerm, "\n")] = 0;

        printf("\n╔════════════════════════════════════════════════════════════════════╗\n");
        printf("║                     SEARCH RESULTS                                ║\n");
        printf("╚════════════════════════════════════════════════════════════════════╝\n");

        for (int i = 0; i < library.count; i++) {
            if (strstr(library.books[i].title, searchTerm) != NULL) {
                Book *book = &library.books[i];
                printf("\nBook ID: %d\n", book->id);
                printf("Title: %s\n", book->title);
                printf("Author: %s\n", book->author);
                printf("ISBN: %s\n", book->isbn);
                printf("Year: %d\n", book->year);
                printf("Price: $%.2f\n", book->price);
                printf("Quantity: %d\n", book->quantity);
                printf("─────────────────────────────────────────────────────────────────────\n");
                found++;
            }
        }
    } else if (choice == 2) {
        printf("Enter Author Name: ");
        fgets(searchTerm, 100, stdin);
        searchTerm[strcspn(searchTerm, "\n")] = 0;

        printf("\n╔════════════════════════════════════════════════════════════════════╗\n");
        printf("║                     SEARCH RESULTS                                ║\n");
        printf("╚════════════════════════════════════════════════════════════════════╝\n");

        for (int i = 0; i < library.count; i++) {
            if (strstr(library.books[i].author, searchTerm) != NULL) {
                Book *book = &library.books[i];
                printf("\nBook ID: %d\n", book->id);
                printf("Title: %s\n", book->title);
                printf("Author: %s\n", book->author);
                printf("ISBN: %s\n", book->isbn);
                printf("Year: %d\n", book->year);
                printf("Price: $%.2f\n", book->price);
                printf("Quantity: %d\n", book->quantity);
                printf("─────────────────────────────────────────────────────────────────────\n");
                found++;
            }
        }
    }

    if (found == 0) {
        printf("❌ No books found matching your search.\n");
    } else {
        printf("✅ Found %d book(s).\n", found);
    }
}

// Update book information
void updateBook() {
    if (library.count == 0) {
        printf("\n📚 The library is empty. No books to update.\n");
        return;
    }

    printf("\n╔═════════��══════════════════════════════╗\n");
    printf("║       UPDATE BOOK INFORMATION          ║\n");
    printf("╚════════════════════════════════════════╝\n");

    printf("Enter Book ID to update: ");
    int bookId;
    if (scanf("%d", &bookId) != 1) {
        printf("❌ Invalid ID input!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int index = getBookIndexById(bookId);
    if (index == -1) {
        printf("❌ Book with ID %d not found!\n", bookId);
        return;
    }

    Book *book = &library.books[index];
    printf("\nCurrent Book Information:\n");
    printf("Title: %s\n", book->title);
    printf("Author: %s\n", book->author);
    printf("Price: $%.2f\n", book->price);
    printf("Quantity: %d\n", book->quantity);

    printf("\nWhat would you like to update?\n");
    printf("1. Title\n");
    printf("2. Author\n");
    printf("3. Price\n");
    printf("4. Quantity\n");
    printf("Enter choice (1-4): ");

    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("❌ Invalid input!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    switch (choice) {
        case 1:
            printf("Enter new title: ");
            fgets(book->title, MAX_TITLE_LEN, stdin);
            book->title[strcspn(book->title, "\n")] = 0;
            printf("✅ Title updated successfully!\n");
            break;
        case 2:
            printf("Enter new author: ");
            fgets(book->author, MAX_AUTHOR_LEN, stdin);
            book->author[strcspn(book->author, "\n")] = 0;
            printf("✅ Author updated successfully!\n");
            break;
        case 3:
            printf("Enter new price: $");
            if (scanf("%f", &book->price) != 1) {
                printf("❌ Invalid price input!\n");
                clearInputBuffer();
                return;
            }
            clearInputBuffer();
            printf("✅ Price updated successfully!\n");
            break;
        case 4:
            printf("Enter new quantity: ");
            if (scanf("%d", &book->quantity) != 1) {
                printf("❌ Invalid quantity input!\n");
                clearInputBuffer();
                return;
            }
            clearInputBuffer();
            printf("✅ Quantity updated successfully!\n");
            break;
        default:
            printf("❌ Invalid choice!\n");
    }
}

// Delete a book
void deleteBook() {
    if (library.count == 0) {
        printf("\n📚 The library is empty. No books to delete.\n");
        return;
    }

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║         DELETE A BOOK                  ║\n");
    printf("╚════════════════════════════════════════╝\n");

    printf("Enter Book ID to delete: ");
    int bookId;
    if (scanf("%d", &bookId) != 1) {
        printf("❌ Invalid ID input!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int index = getBookIndexById(bookId);
    if (index == -1) {
        printf("❌ Book with ID %d not found!\n", bookId);
        return;
    }

    printf("\nAre you sure you want to delete:\n");
    printf("Title: %s\n", library.books[index].title);
    printf("Author: %s\n", library.books[index].author);
    printf("Confirm deletion? (Y/N): ");

    char confirm;
    if (scanf("%c", &confirm) != 1) {
        clearInputBuffer();
        printf("❌ Invalid input!\n");
        return;
    }
    clearInputBuffer();

    if (confirm == 'Y' || confirm == 'y') {
        for (int i = index; i < library.count - 1; i++) {
            library.books[i] = library.books[i + 1];
        }
        library.count--;
        printf("✅ Book deleted successfully!\n");
    } else {
        printf("⚠ Deletion cancelled.\n");
    }
}

// View library statistics
void viewBookStatistics() {
    if (library.count == 0) {
        printf("\n📚 The library is empty. No statistics available.\n");
        return;
    }

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║      LIBRARY STATISTICS                ║\n");
    printf("╚════════════════════════════════════════╝\n");

    int totalBooks = 0;
    int totalQuantity = 0;
    float totalValue = 0.0;
    float avgPrice = 0.0;
    float minPrice = library.books[0].price;
    float maxPrice = library.books[0].price;
    int oldestYear = library.books[0].year;
    int newestYear = library.books[0].year;

    for (int i = 0; i < library.count; i++) {
        totalBooks++;
        totalQuantity += library.books[i].quantity;
        totalValue += library.books[i].price * library.books[i].quantity;

        if (library.books[i].price < minPrice)
            minPrice = library.books[i].price;
        if (library.books[i].price > maxPrice)
            maxPrice = library.books[i].price;
        if (library.books[i].year < oldestYear)
            oldestYear = library.books[i].year;
        if (library.books[i].year > newestYear)
            newestYear = library.books[i].year;
    }

    avgPrice = totalValue / totalBooks;

    printf("Total Unique Books: %d\n", totalBooks);
    printf("Total Quantity in Stock: %d\n", totalQuantity);
    printf("Total Inventory Value: $%.2f\n", totalValue);
    printf("Average Price per Book: $%.2f\n", avgPrice);
    printf("Lowest Price: $%.2f\n", minPrice);
    printf("Highest Price: $%.2f\n", maxPrice);
    printf("Oldest Publication Year: %d\n", oldestYear);
    printf("Newest Publication Year: %d\n", newestYear);
    printf("╚════════════════════════════════════════╝\n");
}

// Main function
int main() {
    int choice;
    int running = 1;

    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║   WELCOME TO BOOK MANAGEMENT SYSTEM    ║\n");
    printf("╚════════════════════════════════════════╝\n");

    while (running) {
        displayMenu();

        if (scanf("%d", &choice) != 1) {
            printf("❌ Invalid input! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                viewAllBooks();
                break;
            case 3:
                searchBook();
                break;
            case 4:
                updateBook();
                break;
            case 5:
                deleteBook();
                break;
            case 6:
                viewBookStatistics();
                break;
            case 7:
                printf("\nThank you for using Book Management System!\n");
                printf("Goodbye! 👋\n\n");
                running = 0;
                break;
            default:
                printf("❌ Invalid choice! Please select a valid option (1-7).\n");
        }
    }

    return 0;
}
