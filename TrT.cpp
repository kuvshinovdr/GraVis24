#include <iostream>
#include <memory>

struct TreeNode {
    int value;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;

    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};
// Обход в порядке предварительного (Preorder)
void preorderTraversal(const TreeNode* node) {
    if (node != nullptr) {
        std::cout << node->value << " ";  // Посетить корень
        preorderTraversal(node->left.get());      // Обойти левое поддерево
        preorderTraversal(node->right.get());     // Обойти правое поддерево
    }
}

// Обход в симметричном порядке (Inorder)
void inorderTraversal(const TreeNode* node) {
    if (node != nullptr) {
        inorderTraversal(node->left.get());       // Обойти левое поддерево
        std::cout << node->value << " ";          // Посетить корень
        inorderTraversal(node->right.get());      // Обойти правое поддерево
    }
}

// Обход в порядке постордерного (Postorder)
void postorderTraversal(const TreeNode* node) {
    if (node != nullptr) {
        postorderTraversal(node->left.get());     // Обойти левое поддерево
        postorderTraversal(node->right.get());    // Обойти правое поддерево
        std::cout << node->value << " ";          // Посетить корень
    }
}
int main() {
    // Создание дерева
    auto root = std::make_unique<TreeNode>(1);
    root->left = std::make_unique<TreeNode>(2);
    root->right = std::make_unique<TreeNode>(3);
    root->left->left = std::make_unique<TreeNode>(4);
    root->left->right = std::make_unique<TreeNode>(5);

    std::cout << "Preorder traversal:\n";
    preorderTraversal(root.get());  // Вывод: 1 2 4 5 3
    std::cout << "\nInorder traversal:\n";
    inorderTraversal(root.get());    // Вывод: 4 2 5 1 3
    std::cout << "\nPostorder traversal:\n";
    postorderTraversal(root.get());  // Вывод: 4 5 2 3 1

    return 0;
}
