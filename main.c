#include "unity.h"
#include "address_book.h"
#include "address_book_fops.h"
#include "address_book_menu.h"

// Optional setup/teardown hooks
void setUp(void) {}
void tearDown(void) {}

void test_search_contact(void)
{
    AddressBook test_book;
    ContactInfo contacts[3] = {
        {"Alice", "1234567890", "alice@example.com", 1},
        {"Bob", "9876543210", "bob@example.com", 2},
        {"Charlie", "5555555555", "charlie@example.com", 3}
    };

    test_book.list = contacts;
    test_book.count = 3;

    // Case 1: Match by name
    Status result = search("Alice", &test_book, test_book.count, 1, "", e_search_contact);
    TEST_ASSERT_EQUAL(e_success, result);

    // Case 2: Match by phone
    result = search("9876543210", &test_book, test_book.count, 2, "", e_search_contact);
    TEST_ASSERT_EQUAL(e_success, result);

    // Case 3: Match by email
    result = search("charlie@example.com", &test_book, test_book.count, 3, "", e_search_contact);
    TEST_ASSERT_EQUAL(e_success, result);

    // Case 4: No match
    result = search("Nonexistent", &test_book, test_book.count, 1, "", e_search_contact);
    TEST_ASSERT_EQUAL(e_no_match, result);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_search_contact);

    return UNITY_END();
}
