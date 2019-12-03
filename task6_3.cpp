/*
 * 6_3. Выведите элементы в порядке post-order (снизу вверх).
 * Дано число N < 106 и последовательность целых чисел из [-2^31..2^31] длиной N.
 * Требуется построить бинарное дерево, заданное наивным порядком вставки.
 * Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в
 * правое поддерево root; иначе в левое поддерево root.Рекурсия запрещена.
 */

#include <iostream>
#include <stack>


class Tree {
private:
    struct TreeNode {
        explicit TreeNode(int _value) : value(_value) {}

        int value = 0;
        TreeNode* parent = nullptr;
        TreeNode* left = nullptr;
        TreeNode* right = nullptr;
    };
    Tree::TreeNode* root = nullptr;


public:
    Tree()
    {
        root = new TreeNode(0);
    }
    void set_root_value(int val);

    void Print(TreeNode* node) const;

    TreeNode* add_root(int value);

    void post_order(int param) const;

    void insert(int value);

};


void Tree::set_root_value(int val)
{
    root->value = val;
}
// param = 1 - cout node; param = 0 - delete nodes
void Tree::post_order(int param) const{
    TreeNode* node = root;
    std::stack<TreeNode*> stack;
    TreeNode* visited_node;
    while (!stack.empty() or node != nullptr){
        if (node != nullptr) {
            stack.push(node);
            node = node->left;
        } else {
            TreeNode* top_node = stack.top();
            if (top_node->right != nullptr and visited_node != top_node->right){
                node = top_node->right;
            } else {
                if(param==1) std::cout << top_node->value << ' ';
                visited_node = stack.top();
                stack.pop();
            }
        }
        if(param==0) delete node;
    }
}


Tree::TreeNode* Tree::add_root(int value) {
    if (!root) {
        root = new TreeNode(value);
    }
    return root;
}


void Tree::insert(int value) {
    TreeNode* node;
    node = new TreeNode(value);
    while (root != nullptr) {
        if (node->value >= root->value) {
            if (root->right != nullptr) {
                root = root->right;
            } else {
                node->parent = root;
                root->right = node;
                break;
            }
        } else if (node->value < root->value) {
            if (root->left != nullptr) {
                root = root->left;
            } else {
                node->parent = root;
                root->left = node;
                break;
            }
        }

    }
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

    tree.post_order(1);
    return 0;
}
