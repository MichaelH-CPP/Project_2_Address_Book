/* address_book_menu.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "address_book.h"
#include "address_book_fops.h"
#include "address_book_menu.h"

void menu_header(const char *str) {
    printf("\n####### Address Book #######\n");
    printf("####### %s\n", str);
}

void main_menu(void) {
    menu_header("Features:");
    printf("0. Exit\n1. Add Contact\n2. Search Contact\n3. Edit Contact\n4. Delete Contact\n5. List Contacts\n6. Save\n");
    printf("Please select an option: ");
}

Status add_contacts(AddressBook *address_book) {
    if (address_book->count >= 100) {
        printf("Address book full. Cannot add more contacts.\n");
        return e_fail;
    }
    ContactInfo *new_contact = &address_book->list[address_book->count];
    printf("Enter Name: ");
    fgets(new_contact->name, sizeof(new_contact->name), stdin);
    new_contact->name[strcspn(new_contact->name, "\n")] = 0;

    printf("Enter Phone Number: ");
    fgets(new_contact->phone_numbers, sizeof(new_contact->phone_numbers), stdin);
    new_contact->phone_numbers[strcspn(new_contact->phone_numbers, "\n")] = 0;

    printf("Enter Email Address: ");
    fgets(new_contact->email_addresses, sizeof(new_contact->email_addresses), stdin);
    new_contact->email_addresses[strcspn(new_contact->email_addresses, "\n")] = 0;

    address_book->count++;
    printf("Contact added successfully!\n");
    return e_success;
}

Status search_contact(AddressBook *address_book) {
    char search_name[NAME_LEN];
    printf("Enter name to search: ");
    fgets(search_name, sizeof(search_name), stdin);
    search_name[strcspn(search_name, "\n")] = 0;

    for (int i = 0; i < address_book->count; i++) {
        if (strcmp(address_book->list[i].name, search_name) == 0) {
            printf("Found: %s - %s - %s\n",
                   address_book->list[i].name,
                   address_book->list[i].phone_numbers,
                   address_book->list[i].email_addresses);
            return e_success;
        }
    }
    printf("Contact not found.\n");
    return e_no_match;
}

Status delete_contact(AddressBook *address_book) {
    char delete_name[NAME_LEN];
    printf("Enter name to delete: ");
    fgets(delete_name, sizeof(delete_name), stdin);
    delete_name[strcspn(delete_name, "\n")] = 0;

    for (int i = 0; i < address_book->count; i++) {
        if (strcmp(address_book->list[i].name, delete_name) == 0) {
            for (int j = i; j < address_book->count - 1; j++) {
                address_book->list[j] = address_book->list[j + 1];
            }
            address_book->count--;
            printf("Contact deleted successfully!\n");
            return e_success;
        }
    }
    printf("Contact not found.\n");
    return e_no_match;
}

Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode) {
    printf("Listing contacts...\n");
    if (address_book->count == 0) {
        printf("No contacts available.\n");
        return e_no_match;
    }
    for (int i = 0; i < address_book->count; i++) {
        printf("%d. %s - %s - %s\n",
               i + 1,
               address_book->list[i].name,
               address_book->list[i].phone_numbers,
               address_book->list[i].email_addresses);
    }
    return e_success;
}

Status menu(AddressBook *address_book) {
    int option;
    do {
        main_menu();
        scanf("%d", &option);
        getchar();

        switch (option) {
            case 1:
                add_contacts(address_book);
                break;
            case 2:
                search_contact(address_book);
                break;
            case 3:
                printf("Edit contact feature not implemented yet.\n");
                break;
            case 4:
                delete_contact(address_book);
                break;
            case 5:
                list_contacts(address_book, "Contact List", NULL, "", e_list);
                break;
            case 6:
                save_file(address_book);
                printf("Contacts saved successfully!\n");
                break;
            case 0:
                printf("Exiting Address Book.\n");
                break;
            default:
                printf("Invalid option! Try again.\n");
        }
    } while (option != 0);

    return e_success;
}