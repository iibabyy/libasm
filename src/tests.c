#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct s_list {
    void            *content;
    struct s_list   *next;
}   t_list;

void print_list(t_list *lst) {
    while (lst) {
        printf("[%s] -> ", (char *)lst->content);
        lst = lst->next;
    }
    printf("NULL\n");
}

extern struct s_list *ft_create_elem(void *data);
extern void ft_lst_add_front(t_list **lst, t_list *new);
extern void ft_list_push_front(t_list **begin_list, void *data);

extern size_t ft_strlen(const char *str);
extern char *ft_strcpy(char *dst, char *src);
extern int ft_strcmp(const char *s1, const char *s2);
extern ssize_t ft_write(int fd, const void *buf, size_t count);
extern ssize_t ft_read(int fd, void *buf, size_t count);
extern int ft_atoi_base(const char *str, const char *base);

void test_list_push_front(const char *label, t_list **begin_list, const char *data) {
    printf("▶ %s\n", label);

    ft_list_push_front(begin_list, (void *)data);

    if (!*begin_list) {
        printf("❌ FAIL: list is still NULL after push_front(\"%s\")\n", data);
        return;
    }

    if ((*begin_list)->content == data) {
        printf("✅ PASS: pushed \"%s\" at front\n", data);
    } else {
        printf("❌ FAIL: head content mismatch → got %p (\"%s\"), expected %p (\"%s\")\n",
               (*begin_list)->content, (char *)(*begin_list)->content, data, data);
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

    if (node->content == input && node->next == NULL) {
        printf("✅ PASS: ft_create_elem(\"%s\") → content ok, next=NULL\n", input);
    } else {
        printf("❌ FAIL: ft_create_elem(\"%s\") → content=%p (expected %p), next=%p (expected NULL)\n",
               input, node->content, input, node->next);
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
