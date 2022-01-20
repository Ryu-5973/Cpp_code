// 二叉树遍历(前序)的非递归版本
#include <iostream>
#include <vector>
#include <stack>

struct TreeNode{
    int val;
    TreeNode *left;
    TreeNode *right;
};

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