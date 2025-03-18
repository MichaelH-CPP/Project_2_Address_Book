#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "address_book_fops.h"
#include "address_book_log.h"
#include "address_book_menu.h"
#include "address_book.h"

int get_option(int type, const char *msg)
{
	/*
	 * Mutilfuction user intractions like
	 * Just an enter key detection
	 * Read an number
	 * Read a charcter
	 */ 

	/* Fill the code to add above functionality */
	char input[10];
	int option;

	if (msg && *msg){
		printf("%s",msg);
	}
	switch (type){
		case NONE:
			getchar();
			return 0;
		case NUM:
			fgets(input, sizeof(input), stdin);
			return atoi(input);
		case CHAR:
			fgets(input, sizeof(input), stdin);
			return toupper(input[0]);
		default:
			return -1;
	}
}

Status save_prompt(AddressBook *address_book)
{
	char option;

	do
	{
		main_menu();

		option = get_option(CHAR, "\rEnter 'N' to Ignore and 'Y' to Save: ");

		if (option == 'Y')
		{
			save_file(address_book);
			printf("Exiting. Data saved in %s\n", DEFAULT_FILE);

			break;
		}
	} while (option != 'N');

	free(address_book->list);

	return e_success;
}

Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
	/* 
	 * Add code to list all the contacts availabe in address_book.csv file
	 * Should be menu based
	 * The menu provide navigation option if the entries increase the page size
	 */ 

	return e_success;
}

void menu_header(const char *str)
{
	fflush(stdout);

	system("clear");

	printf("#######  Address Book  #######\n");
	if (str != '\0')
	{
		printf("#######  %s\n", str);
	}
}

void main_menu(void)
{
	menu_header("Features:\n");

	printf("0. Exit\n");
	printf("1. Add Contact\n");
	printf("2. Search Contact\n");
	printf("3. Edit Contact\n");
	printf("4. Delete Contact\n");
	printf("5. List Contacts\n");
	printf("6. Save\n");
	printf("\n");
	printf("Please select an option: ");
}

Status menu(AddressBook *address_book)
{
	ContactInfo backup;
	Status ret;
	int option;

	do
	{
		main_menu();

		option = get_option(NUM, "");

		if ((address_book-> count == 0) && (option != e_add_contact))
		{
			get_option(NONE, "No entries found!!. Would you like to add? Use Add Contacts");

			continue;
		}

		switch (option)
		{
			case e_add_contact:
				/* Add your implementation to call add_contacts function here */
				break;
			case e_search_contact:
				search_contact(address_book);
				break;
			case e_edit_contact:
				edit_contact(address_book);
				break;
			case e_delete_contact:
				delete_contact(address_book);
				break;
			case e_list_contacts:
				break;
				/* Add your implementation to call list_contacts function here */
			case e_save:
				save_file(address_book);
				break;
			case e_exit:
				break;
		}
	} while (option != e_exit);

	return e_success;
}

Status add_contacts(AddressBook *address_book)
{
    if (address_book->count >= 100) {
        printf("Address book full. Cannot add more contacts.\n");
        return e_fail;
    }

    ContactInfo *new_contact = &address_book->list[address_book->count];

    printf("Enter Name: ");
    fgets(new_contact->name, sizeof(new_contact->name), stdin);
    new_contact->name[strcspn(new_contact->name, "\n")] = 0; // Remove newline character

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


Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
    int found = 0;

    for (int i = 0; i < address_book->count; i++)
    {
        int match = 0;

        // Search based on the selected field
        if (field == 1 && strcasecmp(address_book->list[i].name, str) == 0)
            match = 1;
        else if (field == 2 && strcasecmp(address_book->list[i].phone_numbers, str) == 0)
            match = 1;
        else if (field == 3 && strcasecmp(address_book->list[i].email_addresses, str) == 0)
            match = 1;

        if (match)
        {
            printf("\nFound Contact:\n");
            printf("Name: %s\nPhone: %s\nEmail: %s\n",
                   address_book->list[i].name,
                   address_book->list[i].phone_numbers,
                   address_book->list[i].email_addresses);
            found = 1;
        }
    }

    if (!found)
    {
        printf("Contact not found.\n");
        return e_no_match;
    }

    return e_success;
}


Status search_contact(AddressBook *address_book)
{
    char search_str[NAME_LEN];
    int search_field;

    printf("\nSearch by:\n");
    printf("1. Name\n");
    printf("2. Phone Number\n");
    printf("3. Email Address\n");
    printf("Enter option: ");
    
    scanf("%d", &search_field);
    getchar(); // Clear buffer

    printf("Enter search term: ");
    fgets(search_str, sizeof(search_str), stdin);
    search_str[strcspn(search_str, "\n")] = 0; // Remove newline

    return search(search_str, address_book, address_book->count, search_field, "", e_search_contact);
}



Status edit_contact(AddressBook *address_book)
{
	/* Add the functionality for edit contacts here */
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}
