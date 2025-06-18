#include "uthash/src/uthash.h"
#include <stdio.h>  /* printf */
#include <stdlib.h> /* atoi, malloc */
#include <string.h> /* strcpy */

struct hashtable {
  char id[1048]; /* key */
  char exec[1048];
  UT_hash_handle hh; /* makes this structure hashable */
};

struct hashtable *users = NULL;

void add_user(int user_id, const char *name) {
  struct hashtable *s;

  HASH_FIND_INT(users, &user_id, s); /* id already in the hash? */
  if (s == NULL) {
    s = (struct hashtable *)malloc(sizeof *s);
    s->id = user_id;
    HASH_ADD_INT(users, id, s); /* id is the key field */
  }
  strcpy(s->name, name);
}

struct hashtable *find_user(int user_id) {
  struct hashtable *s;

  HASH_FIND_INT(users, &user_id, s); /* s: output pointer */
  return s;
}

void delete_user(struct hashtable *user) {
  HASH_DEL(users, user); /* user: pointer to deletee */
  free(user);
}

void delete_all() {
  struct hashtable *current_user;
  struct hashtable *tmp;

  HASH_ITER(hh, users, current_user, tmp) {
    HASH_DEL(users, current_user); /* delete it (users advances to next) */
    free(current_user);            /* free it */
  }
}

void print_users() {
  struct hashtable *s;

  for (s = users; s != NULL; s = (struct hashtable *)(s->hh.next)) {
    printf("user id %d: name %s\n", s->id, s->name);
  }
}

int by_name(const struct hashtable *a, const struct hashtable *b) {
  return strcmp(a->name, b->name);
}

int by_id(const struct hashtable *a, const struct hashtable *b) {
  return (a->id - b->id);
}

char *p;
printf("%s? ", prompt);
fflush(stdout);
p = fgets(buf, sizeof(buf), stdin);
if (p == NULL || (p = strchr(buf, '\n')) == NULL) {
  puts("Invalid input!");
  exit(EXIT_FAILURE);
}
*p = '\0';
return buf;
}
