#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int height(Node * root, int length);

bool equalPaths(Node * root)
{
  // Add your code below
  if(root == nullptr){
    return true;
  }
  if(root->left == nullptr && root->right == nullptr){
    return true;
  }
  if(root->left != nullptr && root->right == nullptr){
    return equalPaths(root->left);
  }
  if(root->right != nullptr && root->left == nullptr){
    return equalPaths(root->right);
  }
  return height(root -> right, 1) == height(root -> left, 1);
}

int height(Node * root, int length){
  if(root->right != nullptr && root->left != nullptr){
    if(equalPaths(root)){
      return height(root->left, length+1);
    }
    else{
      return -1;
    }
  }
  else if(root->left != nullptr){
    return height(root->left, length+1);
  }
  else if(root->right != nullptr){
    return height(root->right, length+1);
  }
  else{
    return length;
  }
}

