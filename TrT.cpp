#include <iostream>
#include <memory>
using namespace std;

struct TreeNode {
    int value;
    unique_ptr<TreeNode> left;
    unique_ptr<TreeNode> right;

    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

void preorderTraversal(const TreeNode* node) {
    if (node != nullptr) {
        cout << node->value << " "; 
        preorderTraversal(node->left.get());     
        preorderTraversal(node->right.get());    
    }
}

void inorderTraversal(const TreeNode* node) {
    if (node != nullptr) {
        inorderTraversal(node->left.get());      
        cout << node->value << " ";          
        inorderTraversal(node->right.get());     
    }
}

void postorderTraversal(const TreeNode* node) {
    if (node != nullptr) {
        postorderTraversal(node->left.get());     
        postorderTraversal(node->right.get());    
        cout << node->value << " ";          
    }
}
int main() {
    auto root = make_unique<TreeNode>(1);
    root->left = make_unique<TreeNode>(2);
    root->right = make_unique<TreeNode>(3);
    root->left->left = make_unique<TreeNode>(4);
    root->left->right = make_unique<TreeNode>(5);

    cout << "Preorder traversal:\n";
    preorderTraversal(root.get());  
    cout << "\nInorder traversal:\n";
    inorderTraversal(root.get());   
    cout << "\nPostorder traversal:\n";
    postorderTraversal(root.get());  

    return 0;
}
