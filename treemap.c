#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"
#include <stdbool.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}

TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap *new = (TreeMap*) malloc(sizeof(TreeMap));
  if (new == NULL){
    return NULL;
  }
  new->root = NULL;
  new->current = NULL;
  new->lower_than = lower_than;
  return new;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) {
  if (!tree->root){
    tree->root = createTreeNode(key,value);
  }
  else{
    tree->current = tree->root;
    while (tree->current){
      
      if (is_equal(tree,tree->current->pair->key,key)){
        return;
      }
      if (tree->lower_than(tree->current->pair->key,key)){
        if (!tree->current->right){
          tree->current->right = createTreeNode(key,value);
          tree->current->right->parent = tree->current;
          tree->current = tree->current->right;
          return;
        }
        tree->current = tree->current->right;
      }
      else{
        if (!tree->current->left){
          tree->current->left = createTreeNode(key,value);
          tree->current->left->parent = tree->current;
          tree->current = tree->current->left;
          return;
        }
        tree->current = tree->current->left;
      }
    }
  }
}

TreeNode * minimum(TreeNode * x){
  while (x->left != NULL){
    x = x->left;
  }
  return x;
}

void removeNode(TreeMap * tree, TreeNode* node){
  if (node == NULL) return;
  
  if (node->left == NULL && node->right == NULL){ //sin hijos
    if (node == tree->root){
      tree->root = NULL;
    }
    else{
      if (node == node->parent->left){
        node->parent->left = NULL;
        free(node);
      }
      else{
        node->parent->right = NULL;
        free(node);
      }
    }
  }
  
  if (node->left == NULL || node->right == NULL){ //1 hijo
    TreeNode *hijo;
    if (node->left != NULL) hijo = node->left;
    else hijo = node->right;
    hijo->parent = node->parent;
    if (node->parent == NULL){
      tree->root = NULL;
    }
    else if (node == node->parent->left){
      if (node->right > node->parent){
        node->parent->right = hijo;
      }
      else{
        node->parent->left = hijo;
      }
    }
    free(node);
  }
  if (node->left != NULL && node->right != NULL){ //2 hijos
    TreeNode *hijoMen = node->left;
    if (hijoMen->left == NULL && hijoMen->right == NULL){
      node->parent->right = node->right;
      hijoMen = NULL;
      free(node);
    }
    else{
      node->parent->right = hijoMen->right;
      hijoMen = NULL;
      free(node);
    }
  }
}


void eraseTreeMap(TreeMap * tree, void* key){
  if (tree == NULL || tree->root == NULL) return;
  if (searchTreeMap(tree, key) == NULL) return;
  TreeNode* node = tree->current;
  removeNode(tree, node);
}

Pair * searchTreeMap(TreeMap * tree, void* key) {
  tree->current = tree->root;
  if (tree->root == NULL) return NULL;
  while (tree->current != NULL){
    if (is_equal(tree,tree->current->pair->key,key)){
      return tree->current->pair;
    }
    if (tree->lower_than(tree->current->pair->key,key)){
      tree->current = tree->current->right;
    }
    else{
      tree->current = tree->current->left;
    }
  }
  return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) {
  TreeNode* node = tree->root;
  Pair *dato = tree->root->pair;
  while (node != NULL){
    int comparador = tree->lower_than(key,node->pair->key);
    if (comparador < 0){
      if (dato == NULL || tree->lower_than(node->pair->key,dato->key)){
        dato = node->pair;
      } 
      node = node->left;
    }
    else{
      node = node->right;
    }
  }
  return dato;
}

Pair * firstTreeMap(TreeMap * tree) {
  if (tree->root == NULL) return NULL;
  tree->current = tree->root;
  while (tree->current->left != NULL){
    tree->current = tree->current->left;
  }
  return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  return NULL;
}
