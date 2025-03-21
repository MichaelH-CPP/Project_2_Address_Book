#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "address_book_fops.h"
// #include "address_book_log.h"
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

	if (msg && *msg)
	{
		printf("%s", msg);
	}
	switch (type)
	{
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

		if (toupper(option) == 'Y')
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
	int j = 0; // Counter for number of contacts printed
	for (i = 0; i < address_book->count; i++)
	{
		j++; // Increment contact counter
		printf("%d. Name: %s\nPhone: %s\nEmail: %s\n", i + 1,
			   address_book->list[i].name,
			   address_book->list[i].phone_numbers,
			   address_book->list[i].email_addresses); // Prints the contact information up to 5 times
		if (j >= 5 && address_book->count > i + 1)
		{ // After printing 5 contacts, if there are more contacts, prompt user
			printf("Next Page[n]\n");
			printf("Previous Page[p]\n");
			printf("Quit[q]]\n");
			printf("Choose an option: \n");
			option = get_option(CHAR, ""); // Read user input
			if (option == 'q')
			{ // If q, break out of if and for loop
				break;
			}
			else if (option == 'n')
			{ // If n, reset contact counter and continue to next 5 contacts
				j = 0;
				if (i + 5 >= address_book->count)
				{ // Check if there are more than 5 contacts left, if not, print error message
					printf("No next page, try again\n");
					i = i - 5;
				}
			}
			else if (option == 'p')
			{
				j = 0;
				i = i - 10; // Go back 10 to print previous 5 contancts
				if (i < 5)
				{ // Check if there is a previous page, if not, print error message
					printf("No previous page, try again\n");
					i = 0;
				}
			}
			else
			{
				printf("Invalid option, try again\n"); // If invalid option, print error message
				i = i - 5;
			}
		}
		else if (j <= 5 && address_book->count == i + 1)
		{ // If there are less than 5 contacts left, give option to quit
			printf("Press [q] to quit: ");
			option = get_option(CHAR, "");
			if (option == 'q')
			{
				break;
			}
			else
			{
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

		if ((address_book->count == 0) && (option != e_add_contact))
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
	if (address_book->count >= MAX_CONTACTS)
	{
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
	fgets(new_contact->phone_numbers, sizeof(new_contact->phone_numbers), stdin);
	new_contact->phone_numbers[strcspn(new_contact->phone_numbers, "\n")] = 0;

	/* Input email address */
	printf("Enter Email Address: ");
	fgets(new_contact->email_addresses, sizeof(new_contact->email_addresses), stdin);
	new_contact->email_addresses[strcspn(new_contact->email_addresses, "\n")] = 0;

	address_book->count++;

	printf("Contact added successfully.\n");
	return e_success;
}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	int found = 0;

	if (field < 1 || field > 3)
	{ // Validate field input
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
			if (mode == e_edit || mode == e_delete)
			{
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
	printf("Enter the name of the contact you would like to edit: ");
	char name[50];
	fgets(name, sizeof(name), stdin);

	ContactInfo *currUser = NULL;
	for (int i = 0; i < address_book->count; i++)
	{
		if (strcmp(address_book->list[i].name, name) == 0)
		{
			currUser = &address_book->list[i];
			break;
		}
	}
	if (currUser == NULL)
	{
		printf("Contact not found.\n");
		return e_no_match;
	}

	else
	{
		int option = 0;
		do
		{
			printf("What would you like to edit?\n");
			printf("1. Name\n");
			printf("2. Phone Number\n");
			printf("3. Email Address\n");
			printf("4. Exit\n");
			fgets(option, sizeof(option), stdin);
			switch (option)
			{
			case 1:
				printf("Enter new name: ");
				char *oldName[50] = currUser->name;
				char *newName[50];
				fgets(newName, sizeof(newName), stdin);
				strcpy(currUser->name, newName);
				printf("Succesfully changed %s to %s\n", oldName, newName);

				break;
			case 2:
				printf("Enter new phone number: ");
				char *oldNum[50] = currUser->phone_numbers;
				char *newNum[50];
				fgets(newNum, sizeof(newNum), stdin);
				strcpy(currUser->phone_numbers, newNum);
				printf("Succesfully changed %d to %d\n", oldNum, newNum);
				break;
			case 3:
				printf("Enter new email address: ");
				char *oldEmail[50] = currUser->email_addresses;
				char *newEmail[50];
				fgets(newEmail, sizeof(newEmail), stdin);
				strcpy(currUser->email_addresses, newEmail);
				printf("Succesfully changed %s to %s\n", oldEmail, newEmail);
				break;
			case 4:
				break;
			default:
				printf("Invalid option, try again\n");
				break;
			}
		} while (option != 4);
		printf("Contact edited successfully.\n");
		return e_success;
	}
}

Status delete_contact(AddressBook *address_book)
{
	if (address_book->count == 0)
	{
		printf("\nNo contacts available to delete.\n");
		return e_no_match;
	}

	char search_term[NAME_LEN];
	printf("\nEnter name, phone number, or email to delete: ");
	scanf("%s", search_term);

	int found_index = -1;
	for (int i = 0; i < address_book->count; i++)
	{
		if (strcmp(address_book->list[i].name[0], search_term) == 0)
		{
			found_index = i;
			break;
		}

		for (int j = 0; j < PHONE_NUMBER_COUNT; j++)
		{
			if (strcmp(address_book->list[i].phone_numbers[j], search_term) == 0)
			{
				found_index = i;
				break;
			}
		}

		for (int k = 0; k < EMAIL_ID_COUNT; k++)
		{
			if (strcmp(address_book->list[i].email_addresses[k], search_term) == 0)
			{
				found_index = i;
				break;
			}
		}
	}

	if (found_index == -1)
	{
		printf("\nNo matching contact found.\n");
		return e_no_match;
	}

	// Confirm deletion
	char confirm;
	printf("\nAre you sure you want to delete this contact? (Y/N): ");
	scanf(" %c", &confirm);
	if (confirm != 'Y' && confirm != 'y')
	{
		printf("\nDeletion canceled.\n");
		return e_success;
	}

	// Shift contacts to remove the deleted contact
	for (int i = found_index; i < address_book->count - 1; i++)
	{
		address_book->list[i] = address_book->list[i + 1];
	}

	address_book->count--;

	printf("\nContact deleted successfully.\n");
	return e_success;
}
