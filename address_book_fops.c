#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book.h"

Status load_file(AddressBook *address_book)
{
	int ret;

	/* 
	 * Check for file existance
	 */

	if (ret == 0)
	{
		/* 
		 * Do the neccessary step to open the file
		 * Do error handling
		 */ 
	}
	else
	{
		/* Create a file for adding entries */
	}

	return e_success;
}

Status save_file(AddressBook *address_book)
{
	f (!address_book || address_book->count == 0)
    ;{
        printf("No contacts to save.\n");
        return e_fail;
    }

    /* Open the file for writing */
    address_book->fp = fopen(DEFAULT_FILE, "w");

    if (address_book->fp == NULL)
    {
        perror("Error opening file for writing");
        return e_fail;
    }

    /* Write contacts to file */
    for (int i = 0; i < address_book->count; i++)
    {
        ContactInfo *contact = &address_book->list[i];

        fprintf(address_book->fp, "%d,%s,", contact->si_no, contact->name[0]);

        /* Write all phone numbers */
        for (int j = 0; j < PHONE_NUMBER_COUNT; j++)
        {
            if (strlen(contact->phone_numbers[j]) > 0)
                fprintf(address_book->fp, "%s,", contact->phone_numbers[j]);
            else
                fprintf(address_book->fp, ",");
        }

        /* Write all email addresses */
        for (int k = 0; k < EMAIL_ID_COUNT; k++)
        {
            if (strlen(contact->email_addresses[k]) > 0)
                fprintf(address_book->fp, "%s,", contact->email_addresses[k]);
            else
                fprintf(address_book->fp, ",");
        }

        fprintf(address_book->fp, "\n");
    }

    /* Close the file */
    fclose(address_book->fp);

    printf("Contacts successfully saved to %s\n", DEFAULT_FILE);
    return e_success;
}
