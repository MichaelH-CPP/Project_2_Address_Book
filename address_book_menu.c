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
	int i;
	char option;
	int j = 0;	//Counter for number of contacts printed
	for (i = 0; i < address_book->count; i++){
		j++;		//Increment contact counter
		 printf("%d. Name: %s\nPhone: %s\nEmail: %s\n", i+1, 
			address_book->list[i].name, 
			address_book->list[i].phone_numbers, 
			address_book->list[i].email_addresses);		//Prints the contact information up to 5 times
		if (j >= 5 && address_book->count > i + 1){		//After printing 5 contacts, if there are more contacts, prompt user
			printf("Next Page[n]\n")
			printf("Previous Page[p]\n")
			printf("Quit[q]]\n")
			printf("Choose an option: \n");
			option = get_option(CHAR, "");				//Read user input
			if (option == 'q'){							//If q, break out of if and for loop
				break;
			}
			else if (option == 'n'){				//If n, reset contact counter and continue to next 5 contacts
				j = 0;
				if (i + 5 >= address_book->count){		//Check if there are more than 5 contacts left, if not, print error message
					printf("No next page, try again\n");
					i = i - 5;
				}
			}
			else if (option == 'p'){	
				j = 0;
				i = i - 10;					//Go back 10 to print previous 5 contancts
				if (i < 5){					//Check if there is a previous page, if not, print error message
					printf("No previous page, try again\n");
					i = 0;
				}
			}
			else {
				printf("Invalid option, try again\n");		//If invalid option, print error message
				i = i - 5;
			}
		}
		else if (j <= 5 && address_book->count == i + 1){		//If there are less than 5 contacts left, give option to quit
			printf("Press [q] to quit: ")
			option = get_option(CHAR, "");
			if (option == 'q'){
				break;
			}
			else {
				printf("Invalid option, try again\n");
				i = i - 5;
			}
		}
	 }

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
				list_contacts(address_book, "", NULL, "", e_list);
				break;
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
    /* Check if address book is full */
    if (address_book->count >= MAX_CONTACTS) {
        printf("Address book full. Cannot add more contacts.\n");
        return e_fail;
    }

    ContactInfo *new_contact = &address_book->list[address_book->count];

    /* Input name */
    printf("Enter Name: ");
    fgets(new_contact->name, sizeof(new_contact->name), stdin);
    new_contact->name[strcspn(new_contact->name, "\n")] = 0;

    /* Input phone number */
    printf("Enter Phone Number: ");
    fgets(new_contact->phone, sizeof(new_contact->phone), stdin);
    new_contact->phone[strcspn(new_contact->phone, "\n")] = 0;

    /* Input email address */
    printf("Enter Email Address: ");
    fgets(new_contact->email, sizeof(new_contact->email), stdin);
    new_contact->email[strcspn(new_contact->email, "\n")] = 0;

    address_book->count++;

    printf("Contact added successfully.\n");
    return e_success;
}


Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
    int found = 0;

    if (field < 1 || field > 3) { // Validate field input
        printf("Invalid search field.\n");
        return e_fail;
    }

    for (int i = 0; i < address_book->count; i++)
    {
        int match = 0;

        // Search based on the selected field (case-insensitive partial match)
        if (field == 1 && strcasestr(address_book->list[i].name, str) != NULL)
            match = 1;
        else if (field == 2 && strcasestr(address_book->list[i].phone_numbers, str) != NULL)
            match = 1;
        else if (field == 3 && strcasestr(address_book->list[i].email_addresses, str) != NULL)
            match = 1;

        if (match)
        {
            printf("\nFound Contact:\n");
            printf("ID: %d\n", address_book->list[i].si_no);
            printf("Name: %s\n", address_book->list[i].name);
            printf("Phone: %s\n", address_book->list[i].phone_numbers);
            printf("Email: %s\n", address_book->list[i].email_addresses);
            printf("---------------------------------------------\n");

            found = 1;
            if (mode == e_edit || mode == e_delete) {
                return i; // Return index for edit/delete
            }
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
