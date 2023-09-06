#ifndef BINARYTREE_HPP_
#define BINARYTREE_HPP_

#include <ctime>
#include <initializer_list>
#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>
#include <stack>
#include <functional>
using namespace std;

template <class T>
class BinaryNode
{
public:
    T value;
    BinaryNode* left;
    BinaryNode* right;

public:
    BinaryNode() = default;
    BinaryNode(const T &value) : value(value), left(nullptr), right(nullptr) {}
    ~BinaryNode() = default;
    
};

template <class T>
class BaseTree
{
public:
    typedef BinaryNode<T>   BNode;
    typedef T               value_type;
protected:
    BNode *root;
public:
    BaseTree() = default;
    virtual ~BaseTree() = default;
public:
    void pre_order()
    {
        stack<BNode*> s;
        s.push(root);
        while (s.size())
        {
            auto tmp = s.top();
            cout << tmp -> value << " ";
            s.pop();
            if (tmp -> right)
                s.push(tmp -> right);
            if (tmp -> left)
                s.push(tmp -> left);
        }
        cout << endl;
    }

    void in_order()
    {
        stack<BNode*> s;
        auto node = root;
        while (node != nullptr || s.size())
        {
            while (node != nullptr)
            {
                s.push(node);
                node = node -> left;
            }
            if (s.size())
            {
                node = s.top();
                cout << node -> value << " ";
                s.pop();
                node = node -> right;
            }
        }
        cout << endl;
    }

    void post_order()
    {
        stack<BNode*> s;
        auto curr = root;
        BNode *pre = nullptr;
        s.push(root);
        while (s.size())
        {
            curr = s.top();
            if ((curr -> left == nullptr && curr -> right == nullptr) || 
                (pre != nullptr && (pre == curr -> left || pre == curr -> right)))
            {
                cout << curr -> value << " ";
                pre = curr;
                s.pop();
                
            }
            else 
            {
                if (curr -> right)
                    s.push(curr -> right);
                if (curr -> left)
                    s.push(curr -> left);
            }
        }
        cout << endl;
        
    }

public:
    void pre_order_rec()
    {
        pre_order_impl(root);
        cout << endl;
    }
    void in_order_rec()
    {
        in_order_impl(root);
        cout << endl;
    }
    void post_order_rec()
    {
        post_order_impl(root);
        cout << endl;
    }
    void level_order()
    {
        level_order_impl(root);
        cout << endl;
    }

    int get_all_num()
    {
        return get_all_num_impl(root);
    }

    int get_all_leaf()
    {
        return get_all_leaf_impl(root);
    }

    int get_height()
    {
        return get_height_impl(root);
    }
    
private:
    virtual void freeBinaryTree(BNode *node) = 0;
    
    
private:
    void pre_order_impl(BNode *node)
    {
        if (node == nullptr) return;
        cout << node -> value << " ";
        pre_order_impl(node -> left);
        pre_order_impl(node -> right);
    }

    void in_order_impl(BNode *node)
    {
        if (node == nullptr) return;
        in_order_impl(node -> left);
        cout << node -> value << " ";
        in_order_impl(node -> right);
    }

    void post_order_impl(BNode *node)
    {
        if (node == nullptr) return;
        post_order_impl(node -> left);
        post_order_impl(node -> right);
        cout << node -> value << " ";
    }
    
    void level_order_impl(BNode *node)
    {
        if (node == nullptr) return;
        queue<BNode*> q;
        q.push(node);
        while (q.size())
        {
            auto node = q.front();
            q.pop();
            cout << node -> value << " ";
            if (node -> left) 
                q.push(node -> left);
            if (node -> right)
                q.push(node -> right);
        }
        
    }

    int get_all_num_impl(BNode *node)
    {
        if (node == nullptr) return 0;
        return get_all_num_impl(node -> left) + get_all_num_impl(node -> right) + 1;
    }

    int get_all_leaf_impl(BNode *node)
    {
        if (node == nullptr) return 0;
        if (node -> left == nullptr && node -> right == nullptr) return 1;
        return get_all_leaf_impl(node -> left) + get_all_leaf_impl(node -> right);
    }

    int get_height_impl(BNode *node)
    {
        if (node == nullptr) return 0;
        return max(get_height_impl(node -> left), get_height_impl(node -> right)) + 1;
    }

};

template <class T>
class BinaryTree : public BaseTree<T>
{
public:
    typedef BinaryNode<T>   BNode;
    typedef T               value_type;
private:
    using BaseTree<T>::root;
    value_type mark;

public:
    BinaryTree(const vector<T> &vec) 
    {
        mark = vec.back();
        int index = 0;
        root = create_node(vec, index);
    }
    ~BinaryTree() override
    {
        freeBinaryTree(root);
    }

    
private:
    BNode* create_node(const vector<T> &vec, int &index);
    void freeBinaryTree(BNode *node) override;
};

template <class T>
typename BinaryTree<T>::BNode*
BinaryTree<T>::create_node(const vector<T> &vec, int &index)
{
    if (vec[index] == mark)
    {
        ++index;
        return nullptr;
    }
    auto node = new BinaryNode(vec[index++]);
    node -> left = create_node(vec, index);
    node -> right = create_node(vec, index);
    return node;
}

template <class T>
void
BinaryTree<T>::freeBinaryTree(typename BinaryTree<T>::BNode *node)
{
    if (node == nullptr)
        return;
    BNode* left_node = node -> left;
    BNode* right_node = node -> right;

    delete node;
    freeBinaryTree(left_node);
    freeBinaryTree(right_node);
    
}

template <class T>
class BinarySearchTree : public BaseTree<T>
{
public:
    using BaseTree<T>::root;
    typedef BinaryNode<T>   BNode;
private:
public:
    BinarySearchTree(const initializer_list<T> &li)
    {
        auto curr = li.begin();
        root = new BNode(*curr);
        ++curr;
        for (;curr != li.end(); ++curr)
        {
            create_node(*curr, root);
        }
        
    }
    ~BinarySearchTree() override
    {
        freeBinaryTree(root); 
    }

public:
    // 查找元素， 如果树包含元素，返回true，否则返回false
    pair<BNode*, bool> find_value(const T &value) { return find_value_impl(value, root); }
    bool delete_value(const T&value) { return delete_value_impl(value, root); }
    pair<T, bool> find_parent(const T &value) 
    {
        auto res = find_parent_impl(value, root, root);
        return {res.first -> value, res.second};
    }

private:
    void create_node(const T &value, BNode *node);
    void freeBinaryTree(BNode *node) override;
    pair<BNode*, bool> find_value_impl(const T &value, BNode *node);
    pair<BNode*, bool> find_parent_impl(const T &value, BNode *node, BNode *pNode);
    bool delete_value_impl(const T &value, BNode *node);
    void insert_node(BNode *node, BNode *start);
    bool delete_node(BNode *node, BNode *pNode, bool isLeft);
    
    
};

// helper function
template <class T>
void  BinarySearchTree<T>::create_node(const T &value, BNode *node)
{
    if (value > node -> value)
    {
        if (node -> right == nullptr)
        {
            node -> right = new BNode(value);
            return;
        }
        else
        {
            create_node(value, node -> right);
        }
        
    }
    else 
    {
        if (node -> left == nullptr)
        {
            node -> left = new BNode(value);
            return;
        }
        else
        {
            create_node(value, node -> left);
        }
    }    
}

template <class T>
void BinarySearchTree<T>::freeBinaryTree(BNode *node)
{
    if (node == nullptr)
        return;
    auto left_node = node -> left;
    auto right_node = node -> right;
    delete node;
    freeBinaryTree(left_node);
    freeBinaryTree(right_node);
}

template <class T>
pair<typename BinarySearchTree<T>::BNode*, bool> BinarySearchTree<T>::find_value_impl(const T &value, BNode *node)
{
    if (node == nullptr) return {nullptr,false};
    if (node -> value == value) return {node, true};
    if (value > node -> value)
    {
        return find_value_impl(value, node -> right);
    }
    else
    {
        return find_value_impl(value, node -> left);
    }
    return {nullptr, false}; 
}

template <class T>
bool BinarySearchTree<T>::delete_value_impl(const T &value, BNode *node)
{
    auto node_pair = find_parent_impl(value, root, root);
    if (node_pair.second == false) return false;
    if (node_pair.first == nullptr) return false;

    BNode *pNode = node_pair.first;
    BNode *curr;
    bool isLeft;
    if (value == pNode -> left -> value) 
    {
        curr = pNode -> left;
        isLeft = true;
    }
    else if (value == pNode -> right -> value) 
    {
        curr = pNode -> right;
        isLeft = false;
    }

    return delete_node(curr, pNode, isLeft);
}

template <class T>
bool BinarySearchTree<T>::delete_node(BNode *curr, BNode *pNode, bool isLeft)
{
    if (curr -> left == nullptr && curr -> right == nullptr)
    {
        isLeft ? pNode -> left = nullptr : pNode -> right = nullptr; 
        delete curr;
        return true;
    }

    else if (curr -> left == nullptr)
    {
        isLeft ? pNode -> left = curr -> right : pNode -> right = curr -> right;
        delete curr;
        return true;
    }
    else if (curr -> right == nullptr)
    {
        isLeft ? pNode -> left = curr -> left : pNode -> right = curr -> left;
        return true;
        
    }
    else 
    {
        // 找到左子树最大值
        auto mNode = curr -> left;
        BNode *mpNode = curr;
        bool isLeft;
        while (mNode -> right != nullptr)
        {
            mpNode = mNode;
            mNode = mNode -> right;
        }
        // 判断是该节点是父结点的左子树还是右子树
        if (mpNode -> left == nullptr) isLeft = false;
        else (mpNode -> left -> value == mNode ->value) ? (isLeft = true) : (isLeft = false);
        // 交换删除结点和当前结点的值
        auto tmp = curr -> value;
        curr -> value = mNode -> value;
        mNode -> value = tmp;
        return delete_node(mNode, mpNode, isLeft);
    
    }
    
}

// template <class T>
// bool BinarySearchTree<T>::delete_value_impl(const T &value, BNode *node)
// {
//     if (find_value_impl(value, node).first == root) return false;
//     if (!find_value_impl(value, node).second) return false;
//     auto curr = find_value_impl(value, node).first;
//     auto left_node = curr -> left;
//     auto right_node = curr -> right;
//     // cout << curr -> value << ": " << curr -> left -> value << ", " << curr -> right -> value << endl;
//     auto pNode = find_parent_impl(value, root, root).first;
//     if (curr -> value > pNode -> value)
//     {
//         pNode -> right = nullptr;
//     }
//     else
//     {
//         pNode -> left = nullptr;
//     }
//     delete curr;
//     if (left_node)
//     {
//         insert_node(left_node, pNode);
//     }
//     if (right_node)
//     {
//         insert_node(right_node, pNode);
//     }
//     return true;
// }

template <class T>
pair<typename BinarySearchTree<T>::BNode*, bool> BinarySearchTree<T>::find_parent_impl(const T &value, 
                                                                    typename BinarySearchTree<T>::BNode *node, 
                                                                    typename BinarySearchTree<T>::BNode *pNode)
{
    if (node == nullptr) return {nullptr, false};
    if (value == root -> value ) return {nullptr, false};
    if (node -> value == value) return {pNode, true};
    if (value > node -> value)
    {
        pNode = node;
        return find_parent_impl(value, node -> right, pNode);
    }
    else
    {
        pNode = node;
        return find_parent_impl(value, node -> left, pNode);
    }
}

template <class T>
void BinarySearchTree<T>::insert_node(BNode *node, BNode *start)
{
    if (node -> value > start -> value)
    {
        if (start -> right == nullptr)
        {
            start -> right = node;
            return; 
        }
        else
        {
            insert_node(node, start -> right);
        }
    }
    else
    {
        if (start -> left == nullptr)
        {
            start -> left = node;
            return;
        }
        else 
        {
            insert_node(node ,start -> left);
        }
    }
}

#endif
