
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

extern size_t ft_strlen(const char *str);
extern char *ft_strcpy(char *dst, char *src);

void test_strlen(char *input) {
    int expected = strlen(input);
    int got = ft_strlen(input);

    if (expected == got) {
        printf("✅ PASS: \"%s\" → %d\n", input, got);
    } else {
        printf("❌ FAIL: \"%s\" → got %d, expected %d\n", input, got, expected);
    }
}

void check_strlen() {
	test_strlen("");                   // empty string
    test_strlen("a");                  // single char
    test_strlen("Hello");              // normal word
    test_strlen("Hello, world!");      // punctuation
    test_strlen("1234567890");         // numbers
    test_strlen("foo\0bar");           // string with embedded null
    test_strlen("This is a longer string for testing.");  
}

void test_strcpy(const char *input) {
    char buf1[1024];
    char buf2[1024];

    // Run both functions
    strncpy(buf1, input, 1024);
    ft_strcpy(buf2, (char *)input);

    if (strcmp(buf1, buf2) == 0) {
        printf("✅ PASS: \"%s\" → \"%s\"\n", input, buf2);
    } else {
        printf("❌ FAIL: \"%s\" → got \"%s\", expected \"%s\"\n",
               input, buf2, buf1);
    }
}

void check_strcpy() {
    char    big[1024];

    test_strcpy("");                        // empty string
    test_strcpy("a");                       // single char
    test_strcpy("Hello");                   // normal word
    test_strcpy("Hello, world!");           // punctuation
    test_strcpy("1234567890");              // numbers
    test_strcpy("foo\0bar");                // embedded null (only "foo" should copy)
    test_strcpy("This is a longer string for testing.");

    // Edge case: large string
    memset(big, 'X', 1023);
    big[1023] = '\0';
    test_strcpy(big);
}

int main() {
    check_strlen();
    check_strcpy();

	return 0;
}