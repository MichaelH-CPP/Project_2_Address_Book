#include <stdio.h>
#include "address_book_menu.h"
#include "address_book_fops.h"

int main() {
    AddressBook address_book;
    address_book.count = 0;
    load_file(&address_book);  // Load contacts from file
    menu(&address_book);       // Start menu interaction
    save_file(&address_book);  // Save contacts before exiting
    return 0;
}
