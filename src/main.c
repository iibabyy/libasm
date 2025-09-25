
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct s_list {
    void            *data;
    struct s_list   *next;
}   t_list;

void test_atoi_base(const char *str, const char *base, int expected);
void test_strlen(char *input);
void test_strcpy(const char *input);
void test_strcmp(const char *s1, const char *s2);
void test_write(int fd, const char *msg);
void test_read(const char *data);
void test_strdup(const char *input);
void test_create_elem(const char *input);
void test_lst_add_front(t_list *first, t_list *second);
void test_list_push_front(const char *label, t_list **begin_list, const char *data);
void test_list_size(const char *label, t_list *list, int expected);
void test_list_find(const char *label, t_list *begin, t_list *target, t_list *expected);
void test_list_remove(const char *label, t_list **begin, t_list *to_remove, t_list *expected_head);
void test_list_sort(const char *label, t_list **list);
void test_remove_lowest(const char *label, t_list **list);
void test_list_remove_if(const char *label, t_list **list, const char *to_remove);

extern ssize_t ft_write(int fd, const void *buf, size_t count);
extern ssize_t ft_read(int fd, void *buf, size_t count);

extern struct s_list *ft_create_elem(void *data);
extern void ft_list_push_front(t_list **begin_list, void *data);
void print_list(t_list *lst);

void check_strlen();
void check_strcpy();
void check_strcmp();
void check_write();
void check_read();
void check_strdup();
void check_atoi_base();

void check_create_elem();
void check_lst_add_front();
void check_list_push_front();
void check_list_size();
void check_list_find();
void check_list_remove();
void check_list_sort();
void check_remove_lowest();
void check_list_remove_if();

int main() {
    check_strlen();
    check_strcpy();
    check_strcmp();
    check_write();
    check_read();
    check_strdup();
    check_atoi_base();
    check_create_elem();
    check_lst_add_front();
    check_list_push_front();
    check_list_size();
    check_list_find();
    check_list_remove();
    check_remove_lowest();
    check_list_sort();
    check_list_remove_if();

	return 0;
}

void check_list_remove_if() {
    printf("\n=== Tests ft_list_remove_if ===\n");

    t_list *list = NULL;

    // Test 1: empty list
    test_list_remove_if("Empty list", &list, "apple");

    // Test 2: single element (match)
    list = NULL;
    ft_list_push_front(&list, strdup("apple"));
    test_list_remove_if("Single element list (match)", &list, "apple");

    // Test 3: single element (no match)
    ft_list_push_front(&list, strdup("banana"));
    test_list_remove_if("Single element list (no match)", &list, "apple");

    list = NULL;
    // Test 4: multiple elements with matches
    const char *values[] = {"apple", "banana", "apple", "cherry", "apple"};
    for (int i = 0; i < 5; i++) {
        ft_list_push_front(&list, strdup(values[i]));
    }
    test_list_remove_if("Multiple elements (remove \"apple\")", &list, "apple");

    // Test 5: remove node not in list
    test_list_remove_if("Remove \"orange\" (not in list)", &list, "orange");

    // Cleanup
    t_list *tmp;
    while (list) {
        tmp = list->next;
        free(list->data);
        free(list);
        list = tmp;
    }

    printf("=== Fin des tests ft_list_remove_if ===\n\n");
}

void check_remove_lowest() {
    printf("\n=== Tests remove_lowest ===\n");

    t_list *list = NULL;

    // Test 1: empty list
    test_remove_lowest("Empty list", &list);

    // Test 2: single element
    ft_list_push_front(&list, strdup("apple"));
    test_remove_lowest("Single element list", &list);

    // Test 3: multiple elements
    list = NULL;
    const char *values[] = {"orange", "banana", "apple", "grape", "cherry"};
    for (int i = 0; i < 5; i++) {
        ft_list_push_front(&list, strdup(values[i]));
    }
    test_remove_lowest("Multiple elements (expect apple removed)", &list);

    // Test 4: duplicates
    list = NULL;
    const char *dups[] = {"banana", "banana", "apple", "cherry"};
    for (int i = 0; i < 4; i++) {
        ft_list_push_front(&list, strdup(dups[i]));
    }
    test_remove_lowest("List with duplicates (expect apple removed)", &list);

    // Cleanup remaining list
    t_list *tmp;
    while (list) {
        tmp = list->next;
        free(list->data);
        free(list);
        list = tmp;
    }

    printf("=== Fin des tests remove_lowest ===\n\n");
}

void check_list_sort() {
    printf("\n=== Tests ft_list_sort ===\n");

    t_list *list = NULL;

    // Test 1: empty list
    test_list_sort("Empty list", &list);

    // Test 2: single element
    ft_list_push_front(&list, "apple");
    test_list_sort("Single element list", &list);

    // Cleanup
    free(list);
    list = NULL;

    // Test 3: multiple elements
    const char *values[] = {"orange", "banana", "apple", "grape", "cherry"};
    for (int i = 0; i < 5; i++) {
        ft_list_push_front(&list, strdup(values[i]));
    }
    test_list_sort("Multiple elements list", &list);

    // Cleanup
    t_list *tmp;
    while (list) {
        tmp = list->next;
        free(list->data);
        free(list);
        list = tmp;
    }

    // Test 4: already sorted list
    const char *sorted_vals[] = {"apple", "banana", "cherry", "grape", "orange"};
    for (int i = 4; i >= 0; i--) { // push in reverse to get correct order
        ft_list_push_front(&list, strdup(sorted_vals[i]));
    }
    test_list_sort("Already sorted list", &list);

    // Cleanup
    while (list) {
        tmp = list->next;
        free(list->data);
        free(list);
        list = tmp;
    }

    // Test 5: list with duplicates
    const char *dups[] = {"apple", "banana", "apple", "banana", "cherry"};
    for (int i = 0; i < 5; i++) {
        ft_list_push_front(&list, strdup(dups[i]));
    }
    test_list_sort("List with duplicates", &list);

    // Cleanup
    while (list) {
        tmp = list->next;
        free(list->data);
        free(list);
        list = tmp;
    }

    printf("=== Fin des tests ft_list_sort ===\n\n");
}

void check_list_remove() {
    printf("\n=== Tests ft_list_remove ===\n");

    // Build list: n3 -> n2 -> n1
    t_list *n1 = ft_create_elem("one");
    t_list *n2 = ft_create_elem("two");
    t_list *n3 = ft_create_elem("three");
    n2->next = n1;
    n3->next = n2;
    t_list *head = n3;

    // Case 1: remove head (n3)
    test_list_remove("Remove head", &head, n3, n2);
    print_list(head);

    // Case 2: remove middle (n2)
    test_list_remove("Remove middle", &head, n2, n1);
    print_list(head);

    // Case 3: remove tail (n1)
    test_list_remove("Remove tail", &head, n1, NULL);
    print_list(head);

    // Case 4: remove from empty list
    head = NULL;
    test_list_remove("Remove from empty list", &head, n1, NULL);

    // Case 5: remove node not in list
    t_list *m1 = ft_create_elem("alpha");
    t_list *m2 = ft_create_elem("beta");
    t_list *m3 = ft_create_elem("gamma");
    m2->next = m1;
    m3->next = m2;
    head = m3;
    t_list *other = ft_create_elem("not_in_list");

    test_list_remove("Remove non-existing node", &head, other, m3);
    print_list(head);

    // Cleanup
    free(n1);
    free(n2);
    free(n3);
    free(m1);
    free(m2);
    free(m3);
    free(other);

    printf("=== Fin des tests ft_list_remove ===\n\n");
}

void check_list_find() {
    printf("\n=== Tests ft_list_find ===\n");

    // Build a 3-element list
    t_list *n1 = ft_create_elem("one");
    t_list *n2 = ft_create_elem("two");
    t_list *n3 = ft_create_elem("three");
    n2->next = n1;
    n3->next = n2;

    // Case 1: find head
    test_list_find("Find head (n3)", n3, n3, n3);

    // Case 2: find middle
    test_list_find("Find middle (n2)", n3, n2, n2);

    // Case 3: find tail
    test_list_find("Find tail (n1)", n3, n1, n1);

    // Case 4: not in list
    t_list *other = ft_create_elem("other");
    test_list_find("Find non-existing node", n3, other, NULL);

    // Case 5: empty list
    test_list_find("Empty list", NULL, n1, NULL);

    // Cleanup
    free(n1);
    free(n2);
    free(n3);
    free(other);

    printf("=== Fin des tests ft_list_find ===\n\n");
}

void check_list_size() {
    printf("\n=== Tests ft_list_size ===\n");

    // Empty list
    test_list_size("Empty list", NULL, 0);

    // List with 1 element
    t_list *n1 = ft_create_elem("one");
    test_list_size("One element", n1, 1);

    // List with 2 elements
    t_list *n2 = ft_create_elem("two");
    n2->next = n1;
    test_list_size("Two elements", n2, 2);

    // List with 3 elements
    t_list *n3 = ft_create_elem("three");
    n3->next = n2;
    test_list_size("Three elements", n3, 3);

    // Longer list (5 elements)
    t_list *n4 = ft_create_elem("four");
    t_list *n5 = ft_create_elem("five");
    n4->next = n3;
    n5->next = n4;
    test_list_size("Five elements", n5, 5);

    // Cleanup
    free(n1);
    free(n2);
    free(n3);
    free(n4);
    free(n5);

    // --- Stress test: 1000 nodes ---
    printf("\n--- Stress test: 1000 nodes ---\n");
    t_list *big_list = NULL;
    for (int i = 0; i < 1000; i++) {
        // push string numbers like "node_42"
        char *buf = malloc(32);
        snprintf(buf, 32, "node_%d", i);
        ft_list_push_front(&big_list, buf);
    }

    test_list_size("1000 nodes", big_list, 1000);

    // Cleanup (free both node content and nodes)
    t_list *tmp;
    while (big_list) {
        tmp = big_list->next;
        free(big_list->data);
        free(big_list);
        big_list = tmp;
    }

    printf("=== Fin des tests ft_list_size ===\n\n");
}

void check_list_push_front() {
    printf("\n=== Tests ft_list_push_front ===\n");

    t_list *list = NULL;

    // First node
    test_list_push_front("Insert first node", &list, "one");
    print_list(list);

    // Push another at front
    test_list_push_front("Insert second node", &list, "two");
    print_list(list);

    // Push third
    test_list_push_front("Insert third node", &list, "three");
    print_list(list);

    // Edge case: NULL list pointer (should not segfault)
    t_list **null_ptr = NULL;
    ft_list_push_front(null_ptr, "ignored");
    printf("✅ PASS: calling with NULL list pointer didn’t crash\n");

    // Cleanup
    t_list *tmp;
    while (list) {
        tmp = list->next;
        free(list);
        list = tmp;
    }

    printf("=== Fin des tests ft_list_push_front ===\n\n");
}

void check_lst_add_front() {
    printf("\n=== Tests ft_lst_add_front ===\n");

    // Create two single nodes
    t_list *node1 = ft_create_elem("first");
    t_list *node2 = ft_create_elem("second");

    if (!node1 || !node2) {
        printf("❌ FAIL: Could not allocate nodes\n");
        free(node1);
        free(node2);
        return;
    }

    // Add node2 in front of node1
    test_lst_add_front(node1, node2);

    // Edge case: add to empty list
    t_list *empty = NULL;
    t_list *new_node = ft_create_elem("new");
    test_lst_add_front(empty, new_node); // head remains NULL locally, can't test pointer update outside

    free(node1);
    free(node2);
    free(new_node);

    printf("=== Fin des tests ft_lst_add_front ===\n\n");
}

void check_create_elem() {
    printf("\n=== Tests ft_create_elem ===\n");

    test_create_elem("Hello");
    test_create_elem("World");
    test_create_elem("");   // empty string
    test_create_elem(NULL); // NULL pointer as content

    printf("=== Fin des tests ft_create_elem ===\n\n");
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
