#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define CHAR_TO_INDEX(c) ((int)(c) - (int)('a'))
#define MAX_SIZE 26

struct trie
{
    bool endOfWord;
    struct trie* edges[MAX_SIZE];
};

struct trie* trie_create(void)
{
    struct trie* trie = malloc(sizeof(struct trie));
    if (!trie)
    {
        return NULL;
    }

    trie->endOfWord = false;

    for (unsigned i=0; i<MAX_SIZE; i++)
    {
        trie->edges[i] = NULL;
    }

    return trie;
}

void trie_destroy(struct trie* root)
{
    if (root == NULL)
    {
        return;
    }

    for (unsigned i=0; i<MAX_SIZE; i++)
    {
        trie_destroy(root->edges[i]);
    }

    free(root);
}

int trie_insert_helper(struct trie** root, char* word, unsigned sizeOfWord, unsigned level)
{
    if (level == sizeOfWord)
    {
        (*root)->endOfWord = true;
        return 0;
    }

    unsigned index = CHAR_TO_INDEX(word[level]);
    if ((*root)->edges[index] == NULL)
    {
        (*root)->edges[index] = trie_create();
        if ((*root)->edges[index] == NULL)
        {
            return -1;
        }
    }

    return trie_insert_helper(&((*root)->edges[index]), word, sizeOfWord, level+1);
}

int trie_insert(struct trie** root, char* word)
{
    if (root == NULL)
    {
        return -1;
    }

    unsigned sizeOfWord = strlen(word);
    if (sizeOfWord == 0)
    {
        return -1;
    }

    if (*root == NULL)
    {
        struct trie* trie = trie_create();
        if (!trie)
        {
            return -1;
        }
        *root = trie;
    }

    return trie_insert_helper(root, word, sizeOfWord, 0);
}

enum searchType
{
    TYPE_PREFIX,
    TYPE_WORD
};

int trie_search_helper(struct trie* root, char* string, unsigned sizeOfString, unsigned level, enum searchType type)
{
    unsigned index = CHAR_TO_INDEX(string[level]);
    if (root->edges[index] == NULL)
    {
        return -1;
    }

    if (level == sizeOfString -1)
    {
        if (type == TYPE_PREFIX)
        {
            return 0;
        }
        else if (type == TYPE_WORD && root->edges[index]->endOfWord == true)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }

    return trie_search_helper(root->edges[index], string, sizeOfString, level+1, type);
}

int trie_search(struct trie* root, char* string, enum searchType type)
{
    if (!root)
    {
        return -1;
    }

    unsigned sizeOfString = strlen(string);
    if (sizeOfString == 0)
    {
        return -1;
    }

    return trie_search_helper(root, string, sizeOfString, 0, type);
}

int main(void)
{
    struct trie* root = NULL;

    //
    // 1) Test inserting valid words
    //
    assert(trie_insert(&root, "hello") == 0);
    assert(trie_insert(&root, "hey") == 0);
    assert(trie_insert(&root, "hi") == 0);
    assert(trie_insert(&root, "he") == 0);

    // ensuring we test branching from the root
    assert(trie_insert(&root, "cat") == 0);

    // 2) Test corner-case insertions
    // Insert an empty string => should fail
    assert(trie_insert(&root, "") == -1);

    // Insert with null pointer to root pointer => should fail (returns -1)
    {
        struct trie** null_root_ptr = NULL;
        assert(trie_insert(null_root_ptr, "test") == -1);
    }

    //
    // 3) Test searching as a WORD
    //
    // The full words we inserted must be found (returns 0)
    assert(trie_search(root, "hello", TYPE_WORD) == 0);
    assert(trie_search(root, "hey", TYPE_WORD) == 0);
    assert(trie_search(root, "hi", TYPE_WORD) == 0);
    assert(trie_search(root, "cat", TYPE_WORD) == 0);
    assert(trie_search(root, "he", TYPE_WORD) == 0);

    // A substring that is not a full word should fail (returns -1 if TYPE_WORD)
    assert(trie_search(root, "h", TYPE_WORD) == -1);
    assert(trie_search(root, "hel", TYPE_WORD) == -1);

    // A word that doesn’t exist at all
    assert(trie_search(root, "dog", TYPE_WORD) == -1);
    assert(trie_search(root, "helix", TYPE_WORD) == -1);

    // Searching with a NULL root
    assert(trie_search(NULL, "hello", TYPE_WORD) == -1);

    //
    // 4) Test searching as a PREFIX
    //
    // The prefix "he" is definitely in the trie, so TYPE_PREFIX returns 0
    assert(trie_search(root, "he", TYPE_PREFIX) == 0);

    // "hel" is a prefix of "hello" => 0
    assert(trie_search(root, "hel", TYPE_PREFIX) == 0);

    // Single letter "h" is a prefix of "hello"/"hey"/"hi" => 0
    assert(trie_search(root, "h", TYPE_PREFIX) == 0);

    // "ca" is a prefix of "cat" => 0
    assert(trie_search(root, "ca", TYPE_PREFIX) == 0);

    // Searching for a prefix that doesn’t exist => -1
    assert(trie_search(root, "hez", TYPE_PREFIX) == -1);

    // Searching empty string as prefix => -1 (by implementation)
    assert(trie_search(root, "", TYPE_PREFIX) == -1);

    //
    // 5) Cleanup
    //
    trie_destroy(root);

    printf("All trie tests passed successfully!\n");
    return 0;
}
