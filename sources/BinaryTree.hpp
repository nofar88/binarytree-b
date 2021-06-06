

#ifndef MATALA5_BINARYTREE_H
#define MATALA5_BINARYTREE_H

#include <vector>
#include <iostream>

using namespace std;


namespace ariel {
    template<class T>
    class Node {
    public:
        T value;
        //Pointers = so we can put a zero value
        Node *left;
        Node *right;

        // constructors
        Node(T val) : value(val), left(nullptr), right(nullptr) {}

        Node(T val, Node *r, Node *l) : value(val), left(l), right(r) {}


    };


    template<class T>
    class BinaryTree {
    private:
        Node<T> *root;
    public:

        class iterator {
        private:
            unsigned int current;
            vector<Node<T> *> list;

        public:
            //Ways to cross a tree
            void inorder(Node<T> *node) {
                if (node->left != nullptr) {
                    inorder(node->left);
                }
                list.push_back(node);
                if (node->right != nullptr) {
                    inorder(node->right);
                }
            }


            void preorder(Node<T> *node) {

                list.push_back(node);
                if (node->left != nullptr) {
                    preorder(node->left);
                }
                if (node->right != nullptr) {
                    preorder(node->right);
                }
            }


            void postorder(Node<T> *node) {

                if (node->left != nullptr) {
                    postorder(node->left);
                }
                if (node->right != nullptr) {
                    postorder(node->right);
                }
                list.push_back(node);
            }

// constructor
            iterator(Node<T> *root, int order = 0) {

//order is meant to tell us which method we want to go through on the tree
                if (order == 0) {
                    if (root != nullptr) {
                        inorder(root);
                    }
                } else if (order == 1) {
                    if (root != nullptr) {
                        preorder(root);
                    }
                } else {
                    if (root != nullptr) {
                        postorder(root);
                    }
                }
                current = 0;//Point to the root that is on the list instead of zero
            }


            T &operator*() const {
                return list[current]->value;
            }

            T *operator->() const {
                return &(list[current]->value);
            }

            iterator &operator++() {
                //We know the tree ends up with the rest of the list at size 0 so we delete the list when the rest of us have reached its maximum size
                if (++current == list.size()) {
                    list.clear();
                }
                return *this;
            }

            iterator operator++(int) {
                iterator tmp = *this;
                current++;
                if (current == list.size()) {
                    list.clear();
                }
                return tmp;
            }

            //Access the node itself to a "circle"
            Node<T> *getNode() {
                return list[current];
            }

            bool operator==(const iterator &rhs) const {
                return !(*this != rhs);
            }

            bool operator!=(const iterator &rhs) const {
                //List size affects,
                //If one of the lists is size 0 then there is no need to check at all,
                //If both are size 0 there is no need to check,
                //And if both are different from size 0 then we would like to check the content
                if (list.size() != 0 && rhs.list.size() != 0) {
                    return list[current] != rhs.list[rhs.current];
                }
                return !(list.size() == 0 && rhs.list.size() == 0);
            }
        };

//cunstractor
        BinaryTree() {
            root = nullptr;
        }

        ~BinaryTree() {
            deletTree(root);
        }

        Node<T> *deepCopyTree(Node<T> *root) {
            if (root != nullptr) {
                Node<T> *newcopy = new Node<T>(root->value);
                if (root->left != nullptr) {
                    newcopy->left = deepCopyTree(root->left);
                }
                if (root->right != nullptr) {
                    newcopy->right = deepCopyTree(root->right);
                }
                return newcopy;
            }
            return nullptr;
        }
//The function is responsible for deleting any existing nodes because we dynamically assign them should know to free their memory
        void deletTree(Node<T> *root) {
            if (root != nullptr) {
                if (root->left != nullptr) {
                     deletTree(root->left);
                }
                if (root->right != nullptr) {
                    deletTree(root->right);
                }
                delete root;
            }
        }


            BinaryTree<T> &operator=(BinaryTree<T> other) {
            if (root != nullptr) { delete root; }
            root = deepCopyTree(other.root);
            return *this;
        }

        BinaryTree(BinaryTree<T> const &other) {
            root = deepCopyTree(other.root);
        }

        BinaryTree(BinaryTree<T> &&other) noexcept {
            root = other.root;
            other.root = nullptr;
        }

        BinaryTree<T> &operator=(BinaryTree<T> &&other) noexcept {
            if (root != nullptr) { delete root; }
            root = other.root;
            other.root = nullptr;
        }



        iterator begin() {
            return iterator(root);
        }

        iterator end() {
            return iterator(nullptr);
        }

        iterator begin_preorder() {
            return iterator(root, 1);
        }

        iterator end_preorder() {
            return iterator(nullptr);

        }

        iterator begin_inorder() {
            return iterator(root, 0);

        }

        iterator end_inorder() {
            return iterator(nullptr);

        }

        iterator begin_postorder() {
            return iterator(root, 2);
        }

        iterator end_postorder() {
            return iterator(nullptr);
        }

        BinaryTree<T> &add_root(T tRoot) {
            if (root != nullptr) {
                root->value = tRoot;
            } else {
                root = new Node<T>(tRoot);
            }
            return *this;
        }

        BinaryTree<T> &add_left(T nodeIn, T nodeAdd) {
            for (iterator it = begin(); it != end(); ++it) {
                if (*it == nodeIn) {
                    if (it.getNode()->left) {
                        it.getNode()->left->value = nodeAdd;
                    } else {
                        it.getNode()->left = new Node<T>(nodeAdd);
                    }
                    return *this;
                }
            }
            throw std::invalid_argument("the node is not exist");
        }


        BinaryTree<T> &add_right(T nodeIn, T nodeAdd) {
            for (iterator it = begin(); it != end(); ++it) {
                if (*it == nodeIn) {
                    if (it.getNode()->right) {
                        it.getNode()->right->value = nodeAdd;
                    } else {
                        it.getNode()->right = new Node<T>(nodeAdd);
                    }
                    return *this;
                }
            }
            throw std::invalid_argument("the node is not exist");
        }

        void printT(const std::string &prefix, const Node<T> *node, bool isLeft) {
            if (node != nullptr) {
                std::cout << prefix;

                if (isLeft) {
                    std::cout << "├──";
                } else {
                    std::cout << "└──";
                }

                // print the value of the node
                std::cout << node->value << std::endl;

                // enter the next tree level - left and right branch
                printT(prefix + (isLeft ? "│   " : "    "), node->left, true);
                printT(prefix + (isLeft ? "│   " : "    "), node->right, false);
            }
        }

        friend ostream &operator<<(ostream &os, BinaryTree<T> &tree) {
            tree.printT("", tree.root, false);
            return os;
        }
    };
}

#endif //MATALA5_BINARYTREE_H
