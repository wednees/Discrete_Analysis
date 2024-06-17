#include <iostream>
#include <vector>
#include <string>

// Нужно найти все пути от-корня-до-листа в любом порядке
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

/*
        10
       /  \
      5    4
     / \    \
    3   6    9
 
 { "10->5->3", "10->5->6", "10->4->9" }
*/

void find_path(TreeNode* node, std::string path, std::vector<std::string>& paths){
    if (node == nullptr){
        return;
    }

    if(!path.empty()){
        path += "->";
    }
    
    path += std::to_string(node->val);
    
    if((node->left == nullptr) && (node->right == nullptr)){
        paths.push_back(path);
    }
    else{
        find_path(node->left, path, paths);
        find_path(node->right, path, paths);
    }
}

std::vector<std::string> binaryTreePaths(TreeNode* root)
{
    std::vector<std::string> paths;
    find_path(root, "", paths);
    return paths;
}

int main(){
    TreeNode a(3), b(6), c(9);
    TreeNode d(5, &a, &b);
    TreeNode e(4, &c, nullptr);
    TreeNode root(10, &d, &e);
    
    std::vector<std::string> res = binaryTreePaths(&root);
    
    for(size_t i = 0; i < res.size(); ++i){
        std::cout<< res[i] << '\n';
    }
    
    return 0;
}