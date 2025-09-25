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

void print_list(t_list *lst) {
    while (lst) {
        printf("[%s] -> ", (char *)lst->data);
        lst = lst->next;
    }
    printf("NULL\n");
}

extern t_list *ft_create_elem(void *data);
extern void ft_lst_add_front(t_list **lst, t_list *new);
extern void ft_list_push_front(t_list **begin_list, void *data);
extern int ft_list_size(t_list *begin_list);
extern t_list *ft_list_find(t_list *begin, t_list *to_find);
extern t_list *ft_list_remove(t_list **begin, t_list *to_remove);
extern t_list *remove_lowest(t_list **begin, int (*cmp)());
extern void ft_list_sort(t_list **begin_list, int (*cmp)());
extern void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *));

extern size_t ft_strlen(const char *str);
extern char *ft_strcpy(char *dst, char *src);
extern int ft_strcmp(const char *s1, const char *s2);
extern ssize_t ft_write(int fd, const void *buf, size_t count);
extern ssize_t ft_read(int fd, void *buf, size_t count);
extern int ft_atoi_base(const char *str, const char *base);

int cmp_int_str(char *a, char *b) {
    return strcmp(a, b);
}

void free_content(void *data) {
    free(data);
}

void test_list_remove_if(const char *label, t_list **list, const char *to_remove) {
    printf("▶ %s\n", label);

    ft_list_remove_if(list, (void *)to_remove, cmp_int_str, free_content);

    // Verify no node matches to_remove
    int found = 0;
    t_list *curr = *list;
    while (curr) {
        if (strcmp((char *)curr->data, to_remove) == 0) {
            found = 1;
            break;
        }
        curr = curr->next;
    }

    if (!found) {
        printf("✅ PASS: all nodes \"%s\" removed\n", to_remove);
    } else {
        printf("❌ FAIL: some nodes \"%s\" still in list\n", to_remove);
    }

    print_list(*list);
}

void test_remove_lowest(const char *label, t_list **list) {
    printf("▶ %s\n", label);

    // Step 1: Find expected lowest node in the current list
    t_list *curr = *list;
    t_list *expected = curr;
    while (curr) {
        if (strcmp((char *)curr->data, (char *)expected->data) < 0) {
            expected = curr;
        }
        curr = curr->next;
    }

    // Step 2: Call remove_lowest
    t_list *removed = remove_lowest(list, cmp_int_str);

    // Step 3: Check correctness
    if (expected == NULL && removed == NULL) {
        printf("✅ PASS: list empty, nothing removed\n");
    } else if (removed == expected) {
        printf("✅ PASS: removed lowest = %s\n", (char *)removed->data);
    } else {
        printf("❌ FAIL: removed %s, expected %s\n",
               removed ? (char *)removed->data : "(null)",
               expected ? (char *)expected->data : "(null)");
    }

    // Step 4: Show new list
    print_list(*list);

    // Step 5: Free removed node
    if (removed) {
        free(removed->data);
        free(removed);
    }
}

void test_list_sort(const char *label, t_list **list) {
    printf("▶ %s\n", label);

    ft_list_sort(list, cmp_int_str);

    // Verify sorted order
    t_list *curr = *list;
    int sorted = 1;
    while (curr && curr->next) {
        if (strcmp((char *)curr->data, (char *)curr->next->data) > 0) {
            sorted = 0;
            break;
        }
        curr = curr->next;
    }

    if (sorted) {
        printf("✅ PASS: list sorted\n");
    } else {
        printf("❌ FAIL: list not sorted\n");
    }

    print_list(*list);
}

void test_list_remove(const char *label, t_list **begin, t_list *to_remove, t_list *expected_head) {
    t_list *got = ft_list_remove(begin, to_remove);

    if (*begin == expected_head) {
        printf("✅ PASS: %s → head=%p, removed=%p\n", label, (void *)*begin, (void *)got);
    } else {
        printf("❌ FAIL: %s → head=%p (expected %p), removed=%p\n",
               label, (void *)*begin, (void *)expected_head, (void *)got);
    }
}

void test_list_find(const char *label, t_list *begin, t_list *target, t_list *expected) {
    t_list *got = ft_list_find(begin, target);

    if (got == expected) {
        printf("✅ PASS: %s → found=%p\n", label, (void *)got);
    } else {
        printf("❌ FAIL: %s → got=%p, expected=%p\n", label, (void *)got, (void *)expected);
    }
}

void test_list_size(const char *label, t_list *list, int expected) {
    int got = ft_list_size(list);

    if (got == expected) {
        printf("✅ PASS: %s → size=%d\n", label, got);
    } else {
        printf("❌ FAIL: %s → got %d, expected %d\n", label, got, expected);
    }
}

void test_list_push_front(const char *label, t_list **begin_list, const char *data) {
    printf("▶ %s\n", label);

    ft_list_push_front(begin_list, (void *)data);

    if (!*begin_list) {
        printf("❌ FAIL: list is still NULL after push_front(\"%s\")\n", data);
        return;
    }

    if ((*begin_list)->data == data) {
        printf("✅ PASS: pushed \"%s\" at front\n", data);
    } else {
        printf("❌ FAIL: head content mismatch → got %p (\"%s\"), expected %p (\"%s\")\n",
               (*begin_list)->data, (char *)(*begin_list)->data, data, data);
    }
}

void test_lst_add_front(t_list *node, t_list *node_to_add) {
    t_list *head = node;
    ft_lst_add_front(&head, node_to_add);


    if (head == node_to_add && head->next == node) {
        printf("✅ PASS: Added node %p in front of %p → head ok, next ok\n", node_to_add, node);
    } else {
        printf("❌ FAIL: head=%p (expected %p), head->next=%p (expected %p)\n",
               head, node_to_add, head ? head->next : NULL, node);
    }
}

void test_create_elem(const char *input) {
    t_list *node = ft_create_elem((void *)input);

    if (!node) {
        printf("❌ FAIL: ft_create_elem(\"%s\") returned NULL\n", input);
        return;
    }

    if (node->data == input && node->next == NULL) {
        printf("✅ PASS: ft_create_elem(\"%s\") → content ok, next=NULL\n", input);
    } else {
        printf("❌ FAIL: ft_create_elem(\"%s\") → content=%p (expected %p), next=%p (expected NULL)\n",
               input, node->data, input, node->next);
    }

    free(node); // cleanup
}

void test_atoi_base(const char *str, const char *base, int expected) {
    int got = ft_atoi_base(str, base);

    if (got == expected) {
        printf("✅ PASS: ft_atoi_base(\"%s\", \"%s\") = %d\n", str, base, got);
    } else {
        printf("❌ FAIL: ft_atoi_base(\"%s\", \"%s\") = %d (expected %d)\n",
               str, base, got, expected);
    }
}

void test_strlen(char *input) {
    int expected = strlen(input);
    int got = ft_strlen(input);

    if (expected == got) {
        printf("✅ PASS: \"%s\" → %d\n", input, got);
    } else {
        printf("❌ FAIL: \"%s\" → got %d, expected %d\n", input, got, expected);
    }
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

void test_strcmp(const char *s1, const char *s2) {
    int expected = strcmp(s1, s2);
    int got = ft_strcmp(s1, s2);

    if ((expected == 0 && got == 0) || (expected < 0 && got < 0) || (expected > 0 && got > 0)) {
        printf("✅ PASS: \"%s\" vs \"%s\" → %d\n", s1, s2, got);
    } else {
        printf("❌ FAIL: \"%s\" vs \"%s\" → got %d, expected %d\n", s1, s2, got, expected);
    }
}

void test_write(int fd, const char *msg) {
    ssize_t expected = write(fd, msg, strlen(msg));
    ssize_t got = ft_write(fd, msg, strlen(msg)); // your version

    if (expected == got) {
        printf("✅ PASS: wrote \"%s\" (%zd bytes)\n", msg, got);
    } else {
        printf("❌ FAIL: \"%s\" → got %zd, expected %zd (errno=%d)\n",
               msg, got, expected, errno);
    }
}

void test_read(const char *data) {
    char buf1[1024];
    char buf2[1024];

    // Create a temporary file with the test data
    FILE *tmp = tmpfile();
    if (!tmp) {
        perror("tmpfile");
        return;
    }
    fwrite(data, 1, strlen(data), tmp);
    rewind(tmp);

    int fd = fileno(tmp);

    // Run libc read for comparison
    ssize_t expected = read(fd, buf1, sizeof(buf1));
    
    // Rewind again to read from the start using your assembly version
    rewind(tmp);
    ssize_t got = ft_read(fd, buf2, sizeof(buf2)); // <-- use ft_read here

    if (expected == got && memcmp(buf1, buf2, expected) == 0) {
        printf("✅ PASS: read \"%s\" (%zd bytes)\n", data, got);
    } else {
        printf("❌ FAIL: read \"%s\" → got %zd (\"%.*s\"), expected %zd (\"%.*s\") errno=%d\n",
               data, got, (int)got, buf2, expected, (int)expected, buf1, errno);
    }

    fclose(tmp);
}

void test_strdup(const char *input) {
    char *libc_copy = strdup(input);
    char *ft_copy = strdup(input); // replace with your assembly strdup if needed

    // If you want to use your assembly version:
    // char *ft_copy = strdup(input); 

    if (!libc_copy || !ft_copy) {
        printf("❌ FAIL: strdup returned NULL for input \"%s\"\n", input);
        free(libc_copy);
        free(ft_copy);
        return;
    }

    if (strcmp(libc_copy, ft_copy) == 0) {
        printf("✅ PASS: strdup(\"%s\") → \"%s\"\n", input, ft_copy);
    } else {
        printf("❌ FAIL: strdup(\"%s\") → got \"%s\", expected \"%s\"\n",
               input, ft_copy, libc_copy);
    }

    free(libc_copy);
    free(ft_copy);
}
