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
	explicit TreeNode(int _value) : value(_value) {}

	int value = 0;
	TreeNode* parent = nullptr;
	TreeNode* left = nullptr;
	TreeNode* right = nullptr;
};


class Tree {
public:
	void Print(TreeNode* node) const;

	TreeNode* add_root(int value);

    void iter_print(TreeNode* node ) const;

	void insert(TreeNode* root, int value);

private:
	TreeNode* root = nullptr;

};


void Tree::iter_print(TreeNode* node ) const{
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
                std::cout << top_node->value << ' ';
                visited_node = stack.top();
                stack.pop();
            }
        }
    }
}


void Tree::Print(TreeNode* node) const{
	if (!node) return;
	Print(node->left);
    Print(node->right);
	std::cout<<node->value << ' ';
}


TreeNode* Tree::add_root(int value) {
	if (!root) {
		root = new TreeNode(value);
	}
	return root;
}


void Tree::insert(TreeNode* root, int value) {
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
    TreeNode* root;
    int N = 0;
    int root_value;
    int node_value;
    std::cin >> N;
    std::cin >> root_value;
	root = tree.add_root(root_value);
	for (int i = 0; i < N - 1; i++){
	    std::cin >> node_value;
        tree.insert(root, node_value);
	}

	tree.iter_print(root);
	return 0;
}