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

    if (level == sizeOfWord - 1)
    {
        (*root)->endOfWord = true;
    }

    return trie_insert_helper(&((*root)->edges[index]), word, sizeOfWord, level+1);
}

int trie_insert(struct trie** root, char* word)
{
    if (root == NULL)
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

    unsigned sizeOfWord = strlen(word);
    if (sizeOfWord == 0)
    {
        return -1;
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
        else if (type == TYPE_WORD && root->endOfWord == true)
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
