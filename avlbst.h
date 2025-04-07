#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child);
    void rotateRight (AVLNode<Key, Value>* node);
    void rotateLeft (AVLNode<Key, Value>* node);
    void removeFix(AVLNode<Key,Value>* n, int diff);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
  AVLNode<Key, Value>* node = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    if(this->root_ == nullptr){
        this->root_ = node;
    }
    else{
        Node<Key, Value>* temp = this->root_;
        while(temp->getKey() != new_item.first){
            if(new_item.first > temp->getKey()){
                if(temp->getRight() == nullptr){
                    temp->setRight(node);
                    node->setParent(temp);
                    break;
                }
                else{
                    temp = temp->getRight();
                }
            }
            else{
                if(temp->getLeft() == nullptr){
                    temp->setLeft(node);
                    node->setParent(temp);
                    break;
                }
                else{
                    temp = temp->getLeft();
                }
            }
        }
        if(temp->getKey() == new_item.first){
            temp->setValue(new_item.second);
        }
    }
  node->setBalance(0);
  //std::cout << "Key: " << node->getKey() << std::endl;
  if(node->getParent() == nullptr){
    return;
  }
  else{
    if(node->getParent()->getBalance() != 0){
      //std::cout<<"no rebalance" << std::endl;
      node->getParent()->setBalance(0);
    }
    else{
      if(node->getKey() < node->getParent()->getKey()){
        node->getParent()->updateBalance(-1);
      }
      else{
        node->getParent()->updateBalance(1);
      }
      //std::cout << "rebalancing" << std::endl;
      insertFix(node->getParent(), node);
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child){
  if(parent == nullptr || child == nullptr){
    return;
  }
  AVLNode<Key, Value>* grandParent = parent->getParent();
  if(grandParent == nullptr){
    //std::cout << "0" << std::endl;
    return;
  }
  if (parent->getKey() < grandParent->getKey()){grandParent->updateBalance(-1);}
  else{grandParent->updateBalance(1);}
  
  if(grandParent->getBalance() == 0){
    //std::cout << "1" << std::endl;
    return;
  }
  else if(grandParent->getBalance() == -1 || grandParent->getBalance() == 1){
    //std::cout << "2" << std::endl;
    insertFix(grandParent, parent);
  }
  else{
    //std::cout << "3" << std::endl;
    if (parent->getKey() < grandParent->getKey()){
      //std::cout << "parent left grandParent" << std::endl;
      if(child->getKey() < parent->getKey()){
        //std::cout << "left zig zig" << std::endl;
        rotateRight(grandParent);
        parent->setBalance(0);
        grandParent->setBalance(0);
      }
      else{
        //std::cout << "left zig zag" << std::endl;
        rotateLeft(parent);
        rotateRight(grandParent);
        if(child->getBalance() == -1){
          parent->setBalance(0);
          grandParent->setBalance(1);
        }
        else if(child->getBalance() == 0){
          parent->setBalance(0);
          grandParent->setBalance(0);
        }
        else{
          parent->setBalance(-1);
          grandParent->setBalance(0);
        }
        child->setBalance(0);
      }
    }
    else{
      //std::cout << "parent right grandParent" << std::endl;
      if(child->getKey() > parent->getKey()){
        //std::cout << "right zig zig" << std::endl;
        rotateLeft(grandParent);
        parent->setBalance(0);
        grandParent->setBalance(0);
      }
      else{
        //std::cout << "right zig zag" << std::endl;
        rotateRight(parent);
        rotateLeft(grandParent);
        if(child->getBalance() == -1){
          parent->setBalance(1);
          grandParent->setBalance(0);
        }
        else if(child->getBalance() == 0){
          parent->setBalance(0);
          grandParent->setBalance(0);
        }
        else{
          parent->setBalance(0);
          grandParent->setBalance(-1);
        }
        child->setBalance(0);
      }
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight (AVLNode<Key, Value>* node){
  if(node == nullptr){
    //std::cout << "null rotation" << std::endl;
    return;
  }
  AVLNode<Key, Value>* lNode = node->getLeft();
  
  lNode->setParent(node->getParent());
  if(node->getParent() != nullptr){
    if(node->getKey() < node->getParent()->getKey()){
      node->getParent()->setLeft(lNode);
    }
    else{
      node->getParent()->setRight(lNode);
    }
  }
  else{
    this->root_ = lNode;
  }

  node->setLeft(lNode->getRight());
  if(node->getLeft() != nullptr){
    node->getLeft()->setParent(node);
  }

  node->setParent(lNode);
  lNode->setRight(node);

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft (AVLNode<Key, Value>* node){
  if(node == nullptr){
    //std::cout << "null rotation" << std::endl;
    return;
  }
  AVLNode<Key, Value>* rNode = node->getRight();
  
  rNode->setParent(node->getParent());
  if(node->getParent() != nullptr){
    if(node->getKey() < node->getParent()->getKey()){
      node->getParent()->setLeft(rNode);
    }
    else{
      node->getParent()->setRight(rNode);
    }
  }
  else{
    this->root_ = rNode;
  }

  node->setRight(rNode->getLeft());
  if(node->getRight() != nullptr){
    node->getRight()->setParent(node);
  }

  node->setParent(rNode);
  rNode->setLeft(node);
  
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{

  if(this->root_ == nullptr){
    return;
  }
  int diff;
  AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
  bool leftChild;
  if(temp == nullptr){
    return;
  }
  //std::cout << temp->getKey() << std::endl;
  if(temp != this->root_){
    leftChild = temp->getParent()->getKey() > temp->getKey();
  }
  if(temp->getLeft() != nullptr && temp->getRight() != nullptr){
    AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(temp));
    leftChild = pred->getParent()->getKey() > pred->getKey();
    nodeSwap(pred, temp);
    //std::cout << "2 child" << std::endl;
  }
  AVLNode<Key, Value>* parent = temp->getParent();
  if(temp->getLeft() == nullptr && temp->getRight() == nullptr){
    if(temp == this->root_){
      this->root_ = nullptr;
    }
    else if(leftChild){
      //std::cout << "left 0 child" << std::endl;
      temp->getParent()->setLeft(nullptr);
      temp->setParent(nullptr);
      diff = 1;
    }
    else{
      //std::cout << "right 0 child" << std::endl;
      temp->getParent()->setRight(nullptr);
      temp->setParent(nullptr);
      diff = -1;
    }
  }
  else if(temp->getLeft() != nullptr && temp->getRight() == nullptr){
    if(temp->getParent() == nullptr){
      this->root_ = temp->getLeft();
      this->root_->setParent(nullptr);
    }
    else{
      //std::cout << "left" << std::endl;
      // std::cout << temp->getLeft()->getKey() << std::endl;
      // std::cout << temp->getParent()->getKey() << std::endl;

      temp->getLeft()->setParent(temp->getParent());
      if(leftChild){
        temp->getParent()->setLeft(temp->getLeft());
        diff = 1;
      }
      else{
        temp->getParent()->setRight(temp->getLeft());
        diff = -1;
      }
      
    }
  }
  else if(temp->getLeft() == nullptr && temp->getRight() != nullptr){
    if(temp->getParent() == nullptr){
      this->root_ = temp->getRight();
      this->root_->setParent(nullptr);
    }
    else{
      //std::cout << "right" << std::endl;
      // std::cout << temp->getRight()->getKey() << std::endl;
      // std::cout << temp->getParent()->getKey() << std::endl;
      temp->getRight()->setParent(temp->getParent ());
      if(leftChild){
        temp->getParent()->setLeft(temp->getRight());
        diff = 1;
      }
      else{
        temp->getParent()->setRight(temp->getRight());
        diff = -1;
      }
    }
  }
  delete temp;
  removeFix(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* n, int diff){
  if(n == nullptr){
    return;
  }
  int ndiff;
  AVLNode<Key, Value>* p = n->getParent();
  if(p != nullptr){
    if(n->getKey() < p->getKey()){
      ndiff = 1;
    }
    else{
      ndiff = -1;
    }
  }
  if(diff == -1){
    if(n->getBalance() + diff == -2){
      AVLNode<Key, Value>* c = n->getLeft();
      if(c->getBalance() == -1){
        rotateRight(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);
      }
      else if(c->getBalance() == 0){
        rotateRight(n);
        n->setBalance(-1);
        c->setBalance(1);
      }
      else{
        AVLNode<Key, Value>* g = c->getRight();
        rotateLeft(c);
        rotateRight(n);
        if(g->getBalance() == 1){
          n->setBalance(0);
          c->setBalance(-1);
          g->setBalance(0);
        }
        else if(g->getBalance() == 0){
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);
        }
        else{
          n->setBalance(1);
          c->setBalance(0);
          g->setBalance(0);
        }
        removeFix(p, ndiff);
      }
    }
    else if(n->getBalance() + diff == -1){
      n->setBalance(-1);
    }
    else{
      n->setBalance(0);
      removeFix(p, ndiff);
    }
  }
  else if(diff == 1){
    if(n->getBalance() + diff == 2){
      AVLNode<Key, Value>* c = n->getRight();
      if(c->getBalance() == 1){
        rotateLeft(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);
      }
      else if(c->getBalance() == 0){
        rotateLeft(n);
        n->setBalance(1);
        c->setBalance(-1);
      }
      else{
        AVLNode<Key, Value>* g = c->getLeft();
        rotateRight(c);
        rotateLeft(n);
        if(g->getBalance() == -1){
          n->setBalance(0);
          c->setBalance(1);
          g->setBalance(0);
        }
        else if(g->getBalance() == 0){
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);
        }
        else{
          n->setBalance(-1);
          c->setBalance(0);
          g->setBalance(0);
        }
        removeFix(p, ndiff);
      }
    }
    else if(n->getBalance() + diff == 1){
      n->setBalance(1);
    }
    else{
      n->setBalance(0);
      removeFix(p, ndiff);
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
