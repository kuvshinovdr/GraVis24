#include <iostream>
#include <memory>

struct TreeNode {
    int value;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;

    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};
// ����� � ������� ���������������� (Preorder)
void preorderTraversal(const TreeNode* node) {
    if (node != nullptr) {
        std::cout << node->value << " ";  // �������� ������
        preorderTraversal(node->left.get());      // ������ ����� ���������
        preorderTraversal(node->right.get());     // ������ ������ ���������
    }
}

// ����� � ������������ ������� (Inorder)
void inorderTraversal(const TreeNode* node) {
    if (node != nullptr) {
        inorderTraversal(node->left.get());       // ������ ����� ���������
        std::cout << node->value << " ";          // �������� ������
        inorderTraversal(node->right.get());      // ������ ������ ���������
    }
}

// ����� � ������� ������������� (Postorder)
void postorderTraversal(const TreeNode* node) {
    if (node != nullptr) {
        postorderTraversal(node->left.get());     // ������ ����� ���������
        postorderTraversal(node->right.get());    // ������ ������ ���������
        std::cout << node->value << " ";          // �������� ������
    }
}
int main() {
    // �������� ������
    auto root = std::make_unique<TreeNode>(1);
    root->left = std::make_unique<TreeNode>(2);
    root->right = std::make_unique<TreeNode>(3);
    root->left->left = std::make_unique<TreeNode>(4);
    root->left->right = std::make_unique<TreeNode>(5);

    std::cout << "Preorder traversal:\n";
    preorderTraversal(root.get());  // �����: 1 2 4 5 3
    std::cout << "\nInorder traversal:\n";
    inorderTraversal(root.get());    // �����: 4 2 5 1 3
    std::cout << "\nPostorder traversal:\n";
    postorderTraversal(root.get());  // �����: 4 5 2 3 1

    return 0;
}
