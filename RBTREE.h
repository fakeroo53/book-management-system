#ifndef RBTREE_H
#define RBTREE_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/**
 * @file RBTREE.h
 * @brief Red-Black Tree implementation for efficient data storage and retrieval
 * 
 * This header file defines the data structures and function declarations
 * for a self-balancing Red-Black Tree (RBT) data structure.
 */

/* Color enumeration for Red-Black Tree nodes */
typedef enum {
    RED = 0,
    BLACK = 1
} Color;

/* Node structure for Red-Black Tree */
typedef struct RBNode {
    int key;                          /* Unique key for the node */
    void *data;                       /* Pointer to associated data */
    struct RBNode *left;              /* Pointer to left child */
    struct RBNode *right;             /* Pointer to right child */
    struct RBNode *parent;            /* Pointer to parent node */
    Color color;                      /* Color of the node (RED or BLACK) */
} RBNode;

/* Red-Black Tree structure */
typedef struct {
    RBNode *root;                     /* Pointer to root node */
    size_t size;                      /* Number of nodes in the tree */
} RBTree;

/* Comparison function type for custom key comparison */
typedef int (*CompareFunc)(int, int);

/* Data copy function type for copying node data */
typedef void* (*CopyDataFunc)(void*);

/* Data free function type for freeing node data */
typedef void (*FreeDataFunc)(void*);

/**
 * @brief Create a new Red-Black Tree
 * @return Pointer to newly created RBTree, or NULL on failure
 */
RBTree* RBTree_Create(void);

/**
 * @brief Destroy a Red-Black Tree and free all resources
 * @param tree Pointer to the RBTree to destroy
 * @param free_data Function pointer to free node data, or NULL
 */
void RBTree_Destroy(RBTree *tree, FreeDataFunc free_data);

/**
 * @brief Insert a new key-value pair into the Red-Black Tree
 * @param tree Pointer to the RBTree
 * @param key The key to insert
 * @param data Pointer to the data associated with the key
 * @return 1 on success, 0 if key already exists, -1 on failure
 */
int RBTree_Insert(RBTree *tree, int key, void *data);

/**
 * @brief Search for a key in the Red-Black Tree
 * @param tree Pointer to the RBTree
 * @param key The key to search for
 * @return Pointer to the data if found, NULL otherwise
 */
void* RBTree_Search(RBTree *tree, int key);

/**
 * @brief Delete a key from the Red-Black Tree
 * @param tree Pointer to the RBTree
 * @param key The key to delete
 * @param free_data Function pointer to free node data, or NULL
 * @return 1 if key was deleted, 0 if key not found, -1 on failure
 */
int RBTree_Delete(RBTree *tree, int key, FreeDataFunc free_data);

/**
 * @brief Get the number of nodes in the Red-Black Tree
 * @param tree Pointer to the RBTree
 * @return Number of nodes in the tree
 */
size_t RBTree_Size(RBTree *tree);

/**
 * @brief Check if the Red-Black Tree is empty
 * @param tree Pointer to the RBTree
 * @return 1 if tree is empty, 0 otherwise
 */
int RBTree_IsEmpty(RBTree *tree);

/**
 * @brief Get the height of the Red-Black Tree
 * @param tree Pointer to the RBTree
 * @return Height of the tree, or 0 if tree is empty
 */
int RBTree_Height(RBTree *tree);

/**
 * @brief Get the minimum key in the Red-Black Tree
 * @param tree Pointer to the RBTree
 * @return Pointer to the data with minimum key, or NULL if tree is empty
 */
void* RBTree_FindMin(RBTree *tree);

/**
 * @brief Get the maximum key in the Red-Black Tree
 * @param tree Pointer to the RBTree
 * @return Pointer to the data with maximum key, or NULL if tree is empty
 */
void* RBTree_FindMax(RBTree *tree);

/**
 * @brief Perform in-order traversal of the Red-Black Tree
 * @param tree Pointer to the RBTree
 * @param callback Function to call for each node (receives key and data)
 * @return Number of nodes traversed, or -1 on failure
 */
int RBTree_InOrderTraversal(RBTree *tree, void (*callback)(int, void*));

/**
 * @brief Perform pre-order traversal of the Red-Black Tree
 * @param tree Pointer to the RBTree
 * @param callback Function to call for each node (receives key and data)
 * @return Number of nodes traversed, or -1 on failure
 */
int RBTree_PreOrderTraversal(RBTree *tree, void (*callback)(int, void*));

/**
 * @brief Perform post-order traversal of the Red-Black Tree
 * @param tree Pointer to the RBTree
 * @param callback Function to call for each node (receives key and data)
 * @return Number of nodes traversed, or -1 on failure
 */
int RBTree_PostOrderTraversal(RBTree *tree, void (*callback)(int, void*));

/**
 * @brief Clear all nodes from the Red-Black Tree
 * @param tree Pointer to the RBTree
 * @param free_data Function pointer to free node data, or NULL
 * @return 0 on success, -1 on failure
 */
int RBTree_Clear(RBTree *tree, FreeDataFunc free_data);

/**
 * @brief Check if a key exists in the Red-Black Tree
 * @param tree Pointer to the RBTree
 * @param key The key to check
 * @return 1 if key exists, 0 otherwise
 */
int RBTree_Contains(RBTree *tree, int key);

/**
 * @brief Update the data associated with an existing key
 * @param tree Pointer to the RBTree
 * @param key The key to update
 * @param data New data pointer
 * @return 1 on success, 0 if key not found, -1 on failure
 */
int RBTree_Update(RBTree *tree, int key, void *data);

/**
 * @brief Get the successor of a given key
 * @param tree Pointer to the RBTree
 * @param key The reference key
 * @return Pointer to data of successor node, or NULL if no successor
 */
void* RBTree_Successor(RBTree *tree, int key);

/**
 * @brief Get the predecessor of a given key
 * @param tree Pointer to the RBTree
 * @param key The reference key
 * @return Pointer to data of predecessor node, or NULL if no predecessor
 */
void* RBTree_Predecessor(RBTree *tree, int key);

/**
 * @brief Find all keys in range [min_key, max_key]
 * @param tree Pointer to the RBTree
 * @param min_key Lower bound of range (inclusive)
 * @param max_key Upper bound of range (inclusive)
 * @param results Array to store resulting data pointers
 * @param max_results Maximum number of results to store
 * @return Number of results found, or -1 on failure
 */
int RBTree_RangeSearch(RBTree *tree, int min_key, int max_key, 
                       void **results, size_t max_results);

/**
 * @brief Verify Red-Black Tree properties (for debugging)
 * @param tree Pointer to the RBTree
 * @return 1 if tree satisfies all RB properties, 0 otherwise
 */
int RBTree_Verify(RBTree *tree);

#endif /* RBTREE_H */
