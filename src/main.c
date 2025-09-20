
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

extern size_t ft_strlen(const char *str);
extern char *ft_strcpy(char *dst, char *src);
extern int ft_strcmp(const char *s1, const char *s2);

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

void test_strcmp(const char *s1, const char *s2) {
    int expected = strcmp(s1, s2);
    int got = ft_strcmp(s1, s2);

    if ((expected == 0 && got == 0) || (expected < 0 && got < 0) || (expected > 0 && got > 0)) {
        printf("✅ PASS: \"%s\" vs \"%s\" → %d\n", s1, s2, got);
    } else {
        printf("❌ FAIL: \"%s\" vs \"%s\" → got %d, expected %d\n", s1, s2, got, expected);
    }
}

void check_strcmp() {
        // Equal strings
    test_strcmp("", "");
    test_strcmp("a", "a");
    test_strcmp("Hello", "Hello");

    // s1 < s2
    test_strcmp("Hello", "World");
    test_strcmp("abc", "abd");

    // s1 > s2
    test_strcmp("World", "Hello");
    test_strcmp("abd", "abc");

    // Different lengths
    test_strcmp("foo", "foobar");
    test_strcmp("foobar", "foo");

    // Embedded null character
    test_strcmp("foo\0bar", "foo\0baz");

    // Large strings
    char *big1 = malloc(1024);
    char *big2 = malloc(1024);
    memset(big1, 'A', 1023); big1[1023] = '\0';
    memset(big2, 'A', 1022); big2[1022] = 'B'; big2[1023] = '\0';
    test_strcmp(big1, big2);
    free(big1);
    free(big2);
}

int main() {
    // check_strlen();
    // check_strcpy();
    check_strcmp();

	return 0;
}