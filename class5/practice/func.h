#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define GN_NONNEG       01  /* Value must be >= 0 */
#define GN_GT_0         02  /* Value must be > 0 */

/* Convert string to integer, checking for errors */
static int getInt(const char *str, int flags, const char *name) {
    long result;
    char *endptr;

    errno = 0;
    result = strtol(str, &endptr, 10);

    if (errno != 0) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }

    if (*endptr != '\0') {
        fprintf(stderr, "%s not an integer\n", name);
        exit(EXIT_FAILURE);
    }

    if ((flags & GN_NONNEG) && result < 0) {
        fprintf(stderr, "%s negative\n", name);
        exit(EXIT_FAILURE);
    }

    if ((flags & GN_GT_0) && result <= 0) {
        fprintf(stderr, "%s <= 0\n", name);
        exit(EXIT_FAILURE);
    }

    return (int) result;
}

/* Display error message and exit */
static void errExitEN(int errnum, const char *message) {
    fprintf(stderr, "%s: %s\n", message, strerror(errnum));
    exit(EXIT_FAILURE);
}