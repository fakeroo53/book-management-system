#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Red-Black Tree Color Enumeration */
typedef enum {
    RED = 0,
    BLACK = 1
} Color;

/* Red-Black Tree Node Structure */
typedef struct RBNode {
    int key;                    /* Book ID or unique key */
    char data[256];             /* Book information/data */
    Color color;                /* Node color: RED or BLACK */
    struct RBNode *left;        /* Left child pointer */
    struct RBNode *right;       /* Right child pointer */
    struct RBNode *parent;      /* Parent node pointer */
} RBNode;

/* Red-Black Tree Structure */
typedef struct {
    RBNode *root;               /* Root of the tree */
    int size;                   /* Number of nodes in tree */
} RBTree;

/**
 * Create a new Red-Black Tree node
 * @param key: The key value for the node
 * @param data: The data to store in the node
 * @return: Pointer to the newly created node, or NULL on failure
 */
RBNode* createNode(int key, const char *data) {
    RBNode *node = (RBNode*)malloc(sizeof(RBNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed for new node\n");
        return NULL;
    }
    
    node->key = key;
    strncpy(node->data, data, sizeof(node->data) - 1);
    node->data[sizeof(node->data) - 1] = '\0';
    node->color = RED;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    
    return node;
}

/**
 * Create an empty Red-Black Tree
 * @return: Pointer to the newly created tree
 */
RBTree* createTree() {
    RBTree *tree = (RBTree*)malloc(sizeof(RBTree));
    if (tree == NULL) {
        fprintf(stderr, "Memory allocation failed for tree\n");
        return NULL;
    }
    
    tree->root = NULL;
    tree->size = 0;
    
    return tree;
}

/**
 * Get the uncle of a given node
 * @param node: The node whose uncle is to be found
 * @return: Pointer to the uncle node, or NULL if it doesn't exist
 */
RBNode* getUncle(RBNode *node) {
    if (node == NULL || node->parent == NULL || node->parent->parent == NULL) {
        return NULL;
    }
    
    RBNode *parent = node->parent;
    RBNode *grandparent = parent->parent;
    
    if (grandparent->left == parent) {
        return grandparent->right;
    } else {
        return grandparent->left;
    }
}

/**
 * Get the sibling of a given node
 * @param node: The node whose sibling is to be found
 * @return: Pointer to the sibling node, or NULL if it doesn't exist
 */
RBNode* getSibling(RBNode *node) {
    if (node == NULL || node->parent == NULL) {
        return NULL;
    }
    
    if (node->parent->left == node) {
        return node->parent->right;
    } else {
        return node->parent->left;
    }
}

/**
 * Perform left rotation on a node
 * @param tree: Pointer to the tree
 * @param node: The node to rotate left
 */
void rotateLeft(RBTree *tree, RBNode *node) {
    if (node == NULL || node->right == NULL) {
        return;
    }
    
    RBNode *right_child = node->right;
    node->right = right_child->left;
    
    if (right_child->left != NULL) {
        right_child->left->parent = node;
    }
    
    right_child->parent = node->parent;
    
    if (node->parent == NULL) {
        tree->root = right_child;
    } else if (node->parent->left == node) {
        node->parent->left = right_child;
    } else {
        node->parent->right = right_child;
    }
    
    right_child->left = node;
    node->parent = right_child;
}

/**
 * Perform right rotation on a node
 * @param tree: Pointer to the tree
 * @param node: The node to rotate right
 */
void rotateRight(RBTree *tree, RBNode *node) {
    if (node == NULL || node->left == NULL) {
        return;
    }
    
    RBNode *left_child = node->left;
    node->left = left_child->right;
    
    if (left_child->right != NULL) {
        left_child->right->parent = node;
    }
    
    left_child->parent = node->parent;
    
    if (node->parent == NULL) {
        tree->root = left_child;
    } else if (node->parent->right == node) {
        node->parent->right = left_child;
    } else {
        node->parent->left = left_child;
    }
    
    left_child->right = node;
    node->parent = left_child;
}

/**
 * Fix Red-Black Tree violations after insertion
 * @param tree: Pointer to the tree
 * @param node: The newly inserted node
 */
void fixInsert(RBTree *tree, RBNode *node) {
    while (node != tree->root && node->parent->color == RED) {
        RBNode *parent = node->parent;
        RBNode *grandparent = parent->parent;
        RBNode *uncle = getUncle(node);
        
        if (parent == grandparent->left) {
            if (uncle != NULL && uncle->color == RED) {
                /* Case 1: Uncle is RED - recolor */
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            } else {
                /* Case 2/3: Uncle is BLACK - rotation needed */
                if (node == parent->right) {
                    /* Case 2: Left-Right case - left rotation on parent */
                    node = parent;
                    rotateLeft(tree, node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }
                /* Case 3: Left-Left case - right rotation on grandparent */
                parent->color = BLACK;
                grandparent->color = RED;
                rotateRight(tree, grandparent);
            }
        } else {
            if (uncle != NULL && uncle->color == RED) {
                /* Case 1: Uncle is RED - recolor */
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            } else {
                /* Case 2/3: Uncle is BLACK - rotation needed */
                if (node == parent->left) {
                    /* Case 2: Right-Left case - right rotation on parent */
                    node = parent;
                    rotateRight(tree, node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }
                /* Case 3: Right-Right case - left rotation on grandparent */
                parent->color = BLACK;
                grandparent->color = RED;
                rotateLeft(tree, grandparent);
            }
        }
    }
    
    tree->root->color = BLACK;
}

/**
 * Insert a new node into the Red-Black Tree
 * @param tree: Pointer to the tree
 * @param key: The key value for the new node
 * @param data: The data to store in the node
 * @return: 1 on success, 0 on failure
 */
int insertNode(RBTree *tree, int key, const char *data) {
    if (tree == NULL || data == NULL) {
        return 0;
    }
    
    RBNode *new_node = createNode(key, data);
    if (new_node == NULL) {
        return 0;
    }
    
    if (tree->root == NULL) {
        tree->root = new_node;
        new_node->color = BLACK;
        tree->size++;
        return 1;
    }
    
    RBNode *current = tree->root;
    RBNode *parent = NULL;
    
    /* Find the appropriate position for insertion */
    while (current != NULL) {
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            /* Key already exists - update data */
            strncpy(current->data, data, sizeof(current->data) - 1);
            current->data[sizeof(current->data) - 1] = '\0';
            free(new_node);
            return 1;
        }
    }
    
    new_node->parent = parent;
    if (key < parent->key) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
    
    tree->size++;
    fixInsert(tree, new_node);
    
    return 1;
}

/**
 * Find the node with minimum key in a subtree
 * @param node: The root of the subtree
 * @return: Pointer to the node with minimum key
 */
RBNode* findMinimum(RBNode *node) {
    if (node == NULL) {
        return NULL;
    }
    
    while (node->left != NULL) {
        node = node->left;
    }
    
    return node;
}

/**
 * Find the node with maximum key in a subtree
 * @param node: The root of the subtree
 * @return: Pointer to the node with maximum key
 */
RBNode* findMaximum(RBNode *node) {
    if (node == NULL) {
        return NULL;
    }
    
    while (node->right != NULL) {
        node = node->right;
    }
    
    return node;
}

/**
 * Find the in-order successor of a node
 * @param node: The node whose successor is to be found
 * @return: Pointer to the successor node
 */
RBNode* findSuccessor(RBNode *node) {
    if (node == NULL) {
        return NULL;
    }
    
    if (node->right != NULL) {
        return findMinimum(node->right);
    }
    
    RBNode *successor = node->parent;
    while (successor != NULL && node == successor->right) {
        node = successor;
        successor = successor->parent;
    }
    
    return successor;
}

/**
 * Fix Red-Black Tree violations after deletion
 * @param tree: Pointer to the tree
 * @param node: The node to fix (or its replacement)
 * @param parent: The parent of the node
 */
void fixDelete(RBTree *tree, RBNode *node, RBNode *parent) {
    while (node != tree->root && (node == NULL || node->color == BLACK)) {
        if (node != NULL && node->parent != NULL) {
            parent = node->parent;
        }
        
        if (node == NULL || node == parent->left) {
            RBNode *sibling = parent->right;
            
            if (sibling == NULL) {
                node = parent;
                parent = parent->parent;
                continue;
            }
            
            /* Case 1: Sibling is RED */
            if (sibling->color == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                rotateLeft(tree, parent);
                sibling = parent->right;
            }
            
            if (sibling != NULL) {
                /* Case 2: Sibling is BLACK with both black children */
                if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                    (sibling->right == NULL || sibling->right->color == BLACK)) {
                    sibling->color = RED;
                    node = parent;
                } else {
                    /* Case 3: Sibling is BLACK, right child is BLACK */
                    if (sibling->right == NULL || sibling->right->color == BLACK) {
                        if (sibling->left != NULL) {
                            sibling->left->color = BLACK;
                        }
                        sibling->color = RED;
                        rotateRight(tree, sibling);
                        sibling = parent->right;
                    }
                    
                    /* Case 4: Sibling is BLACK, right child is RED */
                    if (sibling != NULL) {
                        sibling->color = parent->color;
                        parent->color = BLACK;
                        if (sibling->right != NULL) {
                            sibling->right->color = BLACK;
                        }
                        rotateLeft(tree, parent);
                        node = tree->root;
                    }
                }
            }
        } else {
            RBNode *sibling = parent->left;
            
            if (sibling == NULL) {
                node = parent;
                parent = parent->parent;
                continue;
            }
            
            /* Case 1: Sibling is RED */
            if (sibling->color == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                rotateRight(tree, parent);
                sibling = parent->left;
            }
            
            if (sibling != NULL) {
                /* Case 2: Sibling is BLACK with both black children */
                if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                    (sibling->right == NULL || sibling->right->color == BLACK)) {
                    sibling->color = RED;
                    node = parent;
                } else {
                    /* Case 3: Sibling is BLACK, left child is BLACK */
                    if (sibling->left == NULL || sibling->left->color == BLACK) {
                        if (sibling->right != NULL) {
                            sibling->right->color = BLACK;
                        }
                        sibling->color = RED;
                        rotateLeft(tree, sibling);
                        sibling = parent->left;
                    }
                    
                    /* Case 4: Sibling is BLACK, left child is RED */
                    if (sibling != NULL) {
                        sibling->color = parent->color;
                        parent->color = BLACK;
                        if (sibling->left != NULL) {
                            sibling->left->color = BLACK;
                        }
                        rotateRight(tree, parent);
                        node = tree->root;
                    }
                }
            }
        }
    }
    
    if (node != NULL) {
        node->color = BLACK;
    }
}

/**
 * Delete a node with a given key from the tree
 * @param tree: Pointer to the tree
 * @param key: The key value to delete
 * @return: 1 on success, 0 if key not found
 */
int deleteNode(RBTree *tree, int key) {
    if (tree == NULL || tree->root == NULL) {
        return 0;
    }
    
    /* Find the node to delete */
    RBNode *node = tree->root;
    while (node != NULL) {
        if (key < node->key) {
            node = node->left;
        } else if (key > node->key) {
            node = node->right;
        } else {
            break;
        }
    }
    
    if (node == NULL) {
        return 0; /* Key not found */
    }
    
    RBNode *replacement;
    RBNode *fix_node;
    RBNode *fix_parent;
    Color original_color = node->color;
    
    /* Case 1: Node has no left child */
    if (node->left == NULL) {
        replacement = node->right;
        fix_parent = node->parent;
        
        if (node->parent == NULL) {
            tree->root = replacement;
        } else {
            if (node->parent->left == node) {
                node->parent->left = replacement;
            } else {
                node->parent->right = replacement;
            }
        }
        
        if (replacement != NULL) {
            replacement->parent = node->parent;
        }
        
        fix_node = replacement;
    }
    /* Case 2: Node has no right child */
    else if (node->right == NULL) {
        replacement = node->left;
        fix_parent = node->parent;
        
        if (node->parent == NULL) {
            tree->root = replacement;
        } else {
            if (node->parent->left == node) {
                node->parent->left = replacement;
            } else {
                node->parent->right = replacement;
            }
        }
        
        if (replacement != NULL) {
            replacement->parent = node->parent;
        }
        
        fix_node = replacement;
    }
    /* Case 3: Node has both children */
    else {
        RBNode *successor = findMinimum(node->right);
        original_color = successor->color;
        replacement = successor->right;
        
        if (successor->parent == node) {
            fix_parent = successor;
            fix_node = replacement;
            if (replacement != NULL) {
                replacement->parent = successor;
            }
        } else {
            fix_parent = successor->parent;
            fix_node = replacement;
            successor->parent->left = replacement;
            if (replacement != NULL) {
                replacement->parent = successor->parent;
            }
            
            successor->right = node->right;
            successor->right->parent = successor;
        }
        
        if (node->parent == NULL) {
            tree->root = successor;
        } else {
            if (node->parent->left == node) {
                node->parent->left = successor;
            } else {
                node->parent->right = successor;
            }
        }
        
        successor->parent = node->parent;
        successor->left = node->left;
        successor->left->parent = successor;
        successor->color = node->color;
    }
    
    free(node);
    tree->size--;
    
    if (original_color == BLACK) {
        fixDelete(tree, fix_node, fix_parent);
    }
    
    return 1;
}

/**
 * Search for a node with a given key
 * @param tree: Pointer to the tree
 * @param key: The key value to search for
 * @return: Pointer to the node if found, NULL otherwise
 */
RBNode* searchNode(RBTree *tree, int key) {
    if (tree == NULL) {
        return NULL;
    }
    
    RBNode *current = tree->root;
    
    while (current != NULL) {
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            return current;
        }
    }
    
    return NULL;
}

/**
 * In-order traversal of the tree (Left-Root-Right)
 * @param node: The root node of the subtree to traverse
 * @param callback: Function pointer to call for each node
 */
void inOrderTraversal(RBNode *node, void (*callback)(RBNode*)) {
    if (node == NULL) {
        return;
    }
    
    inOrderTraversal(node->left, callback);
    callback(node);
    inOrderTraversal(node->right, callback);
}

/**
 * Pre-order traversal of the tree (Root-Left-Right)
 * @param node: The root node of the subtree to traverse
 * @param callback: Function pointer to call for each node
 */
void preOrderTraversal(RBNode *node, void (*callback)(RBNode*)) {
    if (node == NULL) {
        return;
    }
    
    callback(node);
    preOrderTraversal(node->left, callback);
    preOrderTraversal(node->right, callback);
}

/**
 * Post-order traversal of the tree (Left-Right-Root)
 * @param node: The root node of the subtree to traverse
 * @param callback: Function pointer to call for each node
 */
void postOrderTraversal(RBNode *node, void (*callback)(RBNode*)) {
    if (node == NULL) {
        return;
    }
    
    postOrderTraversal(node->left, callback);
    postOrderTraversal(node->right, callback);
    callback(node);
}

/**
 * Calculate the height of the tree
 * @param node: The root node of the subtree
 * @return: The height of the tree
 */
int getHeight(RBNode *node) {
    if (node == NULL) {
        return 0;
    }
    
    int left_height = getHeight(node->left);
    int right_height = getHeight(node->right);
    
    return (left_height > right_height ? left_height : right_height) + 1;
}

/**
 * Validate Red-Black Tree properties
 * @param tree: Pointer to the tree
 * @return: 1 if valid, 0 if invalid
 */
int validateRBTree(RBTree *tree) {
    if (tree == NULL) {
        return 0;
    }
    
    if (tree->root == NULL) {
        return 1;
    }
    
    /* Root must be BLACK */
    if (tree->root->color != BLACK) {
        fprintf(stderr, "Validation Error: Root is not BLACK\n");
        return 0;
    }
    
    return 1;
}

/**
 * Free all nodes in the tree recursively
 * @param node: The root node of the subtree to free
 */
void freeTree(RBNode *node) {
    if (node == NULL) {
        return;
    }
    
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

/**
 * Destroy the entire Red-Black Tree
 * @param tree: Pointer to the tree to destroy
 */
void destroyTree(RBTree *tree) {
    if (tree == NULL) {
        return;
    }
    
    freeTree(tree->root);
    free(tree);
}

/**
 * Get the size of the tree
 * @param tree: Pointer to the tree
 * @return: Number of nodes in the tree
 */
int getTreeSize(RBTree *tree) {
    if (tree == NULL) {
        return 0;
    }
    
    return tree->size;
}

/**
 * Check if the tree is empty
 * @param tree: Pointer to the tree
 * @return: 1 if empty, 0 if not empty
 */
int isEmpty(RBTree *tree) {
    if (tree == NULL) {
        return 1;
    }
    
    return tree->root == NULL;
}

/* ============= Example Usage and Testing ============= */

/**
 * Callback function to print node information
 */
void printNode(RBNode *node) {
    if (node == NULL) {
        return;
    }
    
    const char *color = (node->color == RED) ? "RED" : "BLACK";
    printf("Key: %d | Data: %s | Color: %s\n", node->key, node->data, color);
}

/**
 * Main function demonstrating Red-Black Tree usage
 */
int main() {
    printf("===== Red-Black Tree Implementation =====\n\n");
    
    /* Create a new tree */
    RBTree *tree = createTree();
    if (tree == NULL) {
        fprintf(stderr, "Failed to create tree\n");
        return 1;
    }
    
    printf("1. Inserting books into the tree...\n");
    insertNode(tree, 10, "The Great Gatsby");
    insertNode(tree, 5, "To Kill a Mockingbird");
    insertNode(tree, 15, "1984");
    insertNode(tree, 3, "Jane Eyre");
    insertNode(tree, 7, "Wuthering Heights");
    insertNode(tree, 12, "Brave New World");
    insertNode(tree, 20, "The Catcher in the Rye");
    insertNode(tree, 25, "Moby Dick");
    insertNode(tree, 8, "Pride and Prejudice");
    
    printf("Tree size: %d\n\n", getTreeSize(tree));
    
    printf("2. In-order traversal (sorted order):\n");
    inOrderTraversal(tree->root, printNode);
    printf("\n");
    
    printf("3. Pre-order traversal:\n");
    preOrderTraversal(tree->root, printNode);
    printf("\n");
    
    printf("4. Searching for book with key 12:\n");
    RBNode *found = searchNode(tree, 12);
    if (found != NULL) {
        printf("Found: ");
        printNode(found);
    } else {
        printf("Not found\n");
    }
    printf("\n");
    
    printf("5. Tree height: %d\n\n", getHeight(tree->root));
    
    printf("6. Deleting node with key 5...\n");
    deleteNode(tree, 5);
    printf("Tree size after deletion: %d\n\n", getTreeSize(tree));
    
    printf("7. In-order traversal after deletion:\n");
    inOrderTraversal(tree->root, printNode);
    printf("\n");
    
    printf("8. Validating Red-Black Tree properties:\n");
    if (validateRBTree(tree)) {
        printf("Tree is valid!\n");
    } else {
        printf("Tree is invalid!\n");
    }
    
    /* Clean up */
    destroyTree(tree);
    printf("\nTree destroyed and memory freed.\n");
    
    return 0;
}
