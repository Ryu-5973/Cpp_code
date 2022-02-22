// 二叉树遍历的非递归版本
#include <iostream>
#include <vector>
#include <stack>

struct TreeNode{
    int val;
    TreeNode *left;
    TreeNode *right;
};

// 前序遍历
std::vector<int> Traversal(TreeNode *root) {
    std::vector<int> ans;
    if(root == nullptr) return ans;
    std::stack<TreeNode*> stk;
    TreeNode *cur = root;
    while(stk.empty() == false || cur != nullptr) {
        while(cur != nullptr) {
            stk.push(cur);
            ans.push_back(cur->val);
            cur = cur->left;
        }
        cur = stk.top();
        stk.pop();
        stk.push(cur->right);
    }
    return ans;
}

// 中序遍历
std::vector<int> inorderTraversal(TreeNode* root) {
    std::vector<int> ans;
    if(root == nullptr) return ans;
    std::stack<TreeNode*> stk;
    TreeNode *cur = root;
    while(stk.empty() == false || cur != nullptr) {
        while(cur != nullptr) {
            stk.push(cur);
            cur = cur->left;
        }
        cur = stk.top();
        stk.pop();
        ans.push_back(cur->val);
        cur = cur->right;
    }
    return ans;
}

// 后序遍历
std::vector<int> postorderTraversal(TreeNode* root) {
    std::vector<int> ans;
    if(root == nullptr) return ans;
    std::stack<TreeNode*> stk1, stk2;
    stk1.push(root);
    TreeNode *cur = root;
    while(stk1.empty() == false) {
        cur = stk1.top();
        stk1.pop();
        stk2.push(cur);
        if(cur->left != nullptr)stk1.push(cur->left);
        if(cur->right != nullptr) stk1.push(cur->right);
    }
    while(stk2.empty() == false){
        ans.push_back(stk2.top()->val);
        stk2.pop();
    }
    return ans;
}
