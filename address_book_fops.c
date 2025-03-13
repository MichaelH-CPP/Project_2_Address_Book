
/* address_book_fops.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "address_book.h"
#include "address_book_fops.h"

Status load_file(AddressBook *address_book) {
    address_book->fp = fopen(DEFAULT_FILE, "r");
    if (!address_book->fp) {
        printf("No existing contacts found. Creating new file...\n");
        address_book->fp = fopen(DEFAULT_FILE, "w");
        if (!address_book->fp) return e_fail;
        return e_success;
    }
    address_book->count = 0;
    while (fscanf(address_book->fp, "%[^,],%[^,],%[^\n]\n", 
                  address_book->list[address_book->count].name, 
                  address_book->list[address_book->count].phone_numbers, 
                  address_book->list[address_book->count].email_addresses) == 3) {
        address_book->count++;
    }
    fclose(address_book->fp);
    return e_success;
}

Status save_file(AddressBook *address_book) {
    address_book->fp = fopen(DEFAULT_FILE, "w");
    if (!address_book->fp) return e_fail;
    for (int i = 0; i < address_book->count; i++) {
        fprintf(address_book->fp, "%s,%s,%s\n",
                address_book->list[i].name,
                address_book->list[i].phone_numbers,
                address_book->list[i].email_addresses);
    }
    fclose(address_book->fp);
    return e_success;
}
