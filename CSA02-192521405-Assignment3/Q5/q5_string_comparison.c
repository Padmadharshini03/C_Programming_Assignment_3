#include <stdio.h>
#include <string.h>
#include <ctype.h>

int my_strlen(const char s[])
{
    int len = 0;
    while (s[len] != '\0') len++;
    return len;
}

void my_to_upper(char dst[], const char src[])
{
    int i = 0;
    while (src[i] != '\0') {
        dst[i] = (src[i] >= 'a' && src[i] <= 'z') ? (char)(src[i] - ('a' - 'A')) : src[i];
        i++;
    }
    dst[i] = '\0';
}

void my_reverse(char dst[], const char src[])
{
    int n = my_strlen(src);
    for (int i = 0; i < n; i++) {
        dst[i] = src[n - 1 - i];
    }
    dst[n] = '\0';
}

void lib_to_upper(char dst[], size_t cap, const char src[])
{
    strncpy(dst, src, cap - 1);
    dst[cap - 1] = '\0';
    size_t n = strlen(dst);
    for (size_t i = 0; i < n; i++) {
        dst[i] = (char)toupper((unsigned char)dst[i]);
    }
}

int main(void)
{
    const char *tests[] = {
        "  Meet at  the library at 5 PM ",
        "",
        "   ",
        "Madam, I'm Adam!"
    };
    char buf1[64], buf2[64];

    printf("=== Q5: MANUAL CHAR ARRAYS VS <string.h> ===\n");
    for (int t = 0; t < 4; t++) {
        my_to_upper(buf1, tests[t]);
        my_reverse(buf2, tests[t]);
        printf("Input: \"%s\"\n", tests[t]);
        printf("  Manual Upper  : \"%s\"\n", buf1);
        printf("  Manual Reverse: \"%s\"\n", buf2);
        lib_to_upper(buf1, sizeof(buf1), tests[t]);
        printf("  Library Upper : \"%s\" (strcmp diff = %d)\n\n", buf1, strcmp(buf1, buf1));
    }

    /* Safe bounded copy demonstration */
    char small_buf[12];
    const char *long_sms = "Very long student feedback SMS message";
    snprintf(small_buf, sizeof(small_buf), "%s", long_sms);
    printf("Safe snprintf into 12-byte buffer: \"%s\" (null-terminated, zero overflow)\n", small_buf);
    return 0;
}
