/*
 * 6_3. Выведите элементы в порядке post-order (снизу вверх).
 * Дано число N < 106 и последовательность целых чисел из [-2^31..2^31] длиной N.
 * Требуется построить бинарное дерево, заданное наивным порядком вставки.
 * Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в
 * правое поддерево root; иначе в левое поддерево root.Рекурсия запрещена.
 */

#include <iostream>
#include <stack>

struct TreeNode {
    TreeNode(int _value) : value(_value) {}
    ~TreeNode(){
        if(left != nullptr)
            delete left;
        if(right != nullptr)
            delete right;
    }
    int value = 0;
    TreeNode* parent = nullptr;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};


class Visit
{
public:
    Visit(){};
    void operator()(TreeNode *node)
    {
        std::stack<TreeNode*> one;
        std::stack<TreeNode*> two;

        one.push(node);
        while(!one.empty())
        {
            TreeNode* node = one.top();
            one.pop();
            two.push(node);

            if(node->left != nullptr)
                one.push(node->left);

            if(node->right != nullptr)
                one.push(node->right);
        }

        while(!two.empty())
        {
            std::cout << two.top()->value << " ";
            two.pop();
        }
    }
};


class Tree {
private:

    TreeNode* root = nullptr;


public:
    Tree()
    {
        root = new TreeNode(0);
    }
    ~Tree()
    {
        delete root;
    }

    void set_root_value(int val);

    void Print(TreeNode* node) const;

    TreeNode* add_root(int value);
    template <class TVisitor>
    void post_order(TVisitor visit) const;

    void insert(int value);

};


void Tree::set_root_value(int val)
{
    root->value = val;
}
// param = 1 - cout node; param = 0 - delete nodes
template <class TVisitor>
void Tree::post_order(TVisitor visit) const{
    visit(root);
}


TreeNode* Tree::add_root(int value) {
    if (!root) {
        root = new TreeNode(value);
    }
    return root;
}


void Tree::insert(int value) {
    TreeNode* newNode = new TreeNode( value );

    if ( root == 0 )
    {
        root = newNode;
        return;
    }

    TreeNode* nextNode = root;
    while ( true )
    {
        if ( value < nextNode->value )
        {
            // Смотрим налево
            if ( nextNode->left != 0 )
            {
                nextNode = nextNode->left;
            }
            else
            {
                nextNode->left = newNode;
                return;
            }
        }
        else
        {
            // Смотрим направо
            if ( nextNode->right != 0 )
            {
                nextNode = nextNode->right;
            }
            else
            {
                nextNode->right = newNode;
                return;
            }
        }
    };
}


int main() {
    Tree tree;
    int N = 0;
    int root_value;
    int node_value;
    std::cin >> N;
    std::cin >> root_value;
    tree.set_root_value(root_value);
    for (int i = 0; i < N - 1; i++){
        std::cin >> node_value;
        tree.insert(node_value);
    }
    Visit visit;
    tree.post_order(visit);
    return 0;
}
