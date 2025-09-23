#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

extern size_t ft_strlen(const char *str);
extern char *ft_strcpy(char *dst, char *src);
extern int ft_strcmp(const char *s1, const char *s2);
extern ssize_t ft_write(int fd, const void *buf, size_t count);
extern ssize_t ft_read(int fd, void *buf, size_t count);
extern int ft_atoi_base(const char *str, const char *base);

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
