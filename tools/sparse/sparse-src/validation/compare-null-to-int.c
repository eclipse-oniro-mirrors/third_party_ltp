static unsigned int comparison = (void *)0 == 1;
/*
 * check-name: Compare null pointer constant to int
 * check-description: Sparse used to allow this.
 *
 * check-error-start
compare-null-to-int.c:1:44: error: incompatible types for operation (==):
compare-null-to-int.c:1:44:    void *
compare-null-to-int.c:1:44:    int
 * check-error-end
 */
