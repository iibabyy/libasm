
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

void test_atoi_base(const char *str, const char *base, int expected);
void test_strlen(char *input);
void test_strcpy(const char *input);
void test_strcmp(const char *s1, const char *s2);
void test_write(int fd, const char *msg);
void test_read(const char *data);
void test_strdup(const char *input);

extern ssize_t ft_write(int fd, const void *buf, size_t count);
extern ssize_t ft_read(int fd, void *buf, size_t count);

void check_strlen();
void check_strcpy();
void check_strcmp();
void check_write();
void check_read();
void check_strdup();
void check_atoi_base();

int main() {
    check_strlen();
    check_strcpy();
    check_strcmp();
    check_write();
    check_read();
    check_strdup();
    check_atoi_base();

	return 0;
}

void check_atoi_base() {
    printf("\n=== Tests ft_atoi_base ===\n");

    // --- Bases invalides ---
    test_atoi_base("123", "", 0);           // base vide → invalide
    test_atoi_base("123", "1", 0);          // base taille 1 → invalide
    test_atoi_base("123", "112345", 0);     // doublon → invalide
    test_atoi_base("123", " 0123456789", 0);// espace dans la base → invalide
    test_atoi_base("123", "+0123456789", 0);// '+' dans la base → invalide
    test_atoi_base("123", "-0123456789", 0);// '-' dans la base → invalide

    // --- Base décimale ---
    test_atoi_base("42", "0123456789", 42);
    test_atoi_base("   42", "0123456789", 42);  // espaces devant
    test_atoi_base("+42", "0123456789", 42);
    test_atoi_base("-42", "0123456789", -42);
    test_atoi_base("0", "0123456789", 0);
    test_atoi_base("123abc", "0123456789", 123); // stop à 'a'

    // --- Base binaire ---
    test_atoi_base("101", "01", 5);
    test_atoi_base("-101", "01", -5);

    // --- Base hexadécimale ---
    test_atoi_base("1A", "0123456789ABCDEF", 26);
    test_atoi_base("ff", "0123456789abcdef", 255);
    test_atoi_base("-ff", "0123456789abcdef", -255);

    // --- Base octale ---
    test_atoi_base("77", "01234567", 63);

    // --- Base personnalisée ---
    test_atoi_base("abba", "ab", 6);   // "abba" en base2 → 1111
    test_atoi_base("cab", "abc", 19);   // "cab" en base3 → (2*9 + 0*3 + 1)

    printf("=== Fin des tests ft_atoi_base ===\n\n");
}

void check_strlen() {
    printf("\n=== Tests ft_strlen ===\n");

    test_strlen("");                   // empty string
    test_strlen("a");                  // single char
    test_strlen("Hello");              // normal word
    test_strlen("Hello, world!");      // punctuation
    test_strlen("1234567890");         // numbers
    test_strlen("foo\0bar");           // string with embedded null
    test_strlen("This is a longer string for testing.");  

    // Edge case: very large string
    char *big = malloc(5000);
    memset(big, 'X', 4999);
    big[4999] = '\0';
    test_strlen(big);
    free(big);

    printf("=== Fin des tests ft_strlen ===\n\n");
}

void check_strcpy() {
    printf("\n=== Tests ft_strcpy ===\n");

    char big[1024];

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

    // Edge case: copy into uninitialized buffer (safety check)
    char buf[1];
    test_strcpy("X");

    printf("=== Fin des tests ft_strcpy ===\n\n");
}

void check_strcmp() {
    printf("\n=== Tests ft_strcmp ===\n");

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

    // Edge case: completely different first chars
    test_strcmp("Zebra", "Apple");

    free(big1);
    free(big2);

    printf("=== Fin des tests ft_strcmp ===\n\n");
}

void check_write() {
    printf("\n=== Tests ft_write ===\n");

    test_write(1, "Hello, world!\n");
    test_write(1, "Test with numbers: 1234567890\n");
    test_write(1, "Edge case: empty string\n");
    test_write(1, "String with\0embedded nulls?\n");
    
    // Error case: invalid fd
    const char *err = "This should fail\n";
    ssize_t ret = ft_write(-1, err, strlen(err));
    if (ret == -1) {
        printf("✅ PASS: write to invalid fd returned -1 (errno=%d)\n", errno);
    } else {
        printf("❌ FAIL: write to invalid fd → returned %zd\n", ret);
    }

    printf("=== Fin des tests ft_write ===\n\n");
}

void check_read() {
    printf("\n=== Tests ft_read ===\n");

    test_read("");                        // empty file
    test_read("a");                       // single char
    test_read("Hello");                   // small word
    test_read("Hello, world!\n");         // sentence
    test_read("1234567890");              // numbers
    test_read("This is a longer string for testing purposes.");

    // Error case: invalid fd
    char buf[16];
    ssize_t ret = ft_read(-1, buf, sizeof(buf));
    if (ret == -1) {
        printf("✅ PASS: read from invalid fd returned -1 (errno=%d)\n", errno);
    } else {
        printf("❌ FAIL: read from invalid fd → returned %zd\n", ret);
    }

    // Edge case: read zero bytes
    int fd = open("/dev/null", O_RDONLY);
    ret = ft_read(fd, buf, 0);
    if (ret == 0) {
        printf("✅ PASS: read zero bytes returned 0\n");
    } else {
        printf("❌ FAIL: read zero bytes → returned %zd\n", ret);
    }
    close(fd);

    printf("=== Fin des tests ft_read ===\n\n");
}

void check_strdup() {
    printf("\n=== Tests ft_strdup ===\n");

    test_strdup("");                        // empty string
    test_strdup("a");                       // single char
    test_strdup("Hello");                   // normal word
    test_strdup("Hello, world!");           // punctuation
    test_strdup("1234567890");              // numbers
    test_strdup("foo\0bar");                // embedded null (only "foo" should copy)
    test_strdup("This is a longer string for testing purposes.");  

    // Large string
    char *big = malloc(1024);
    if (!big) {
        perror("malloc");
        return;
    }
    memset(big, 'X', 1023);
    big[1023] = '\0';
    test_strdup(big);
    free(big);

    // Edge case: duplicate single whitespace
    test_strdup(" ");

    printf("=== Fin des tests ft_strdup ===\n\n");
}
