#include <vector>
#include <cstdint>
#include <cstddef>
#include <iostream>

/*
 * Даны два отсортированных списка, нужно смерджить одинаковые элементы в один список
 * без повторений.
 * Ввод: l1=[1,2,5,5,6,10], l2=[2,5,8,10,20]
 * Вывод: [2,5,10]
 */

std::vector<int32_t> merge(const std::vector<int32_t> &l1, const std::vector<int32_t> &l2)
{
    
    std::vector<int32_t> result;
    
    size_t it1 = 0;
    size_t it2 = 0;
    
    while((it1 != l1.size()) || (it2 != l2.size())){
        if(l1[it1] == l2[it2]){
            result.push_back( l2[it2] );
            it2++;
            it1++;
        }
        else{
            if(l1[it1] < l2[it2]){
                it1++;
            }
            else{
                it2++;
            }
        }
    }
    
    
    
    return result;
}

int main(){
    std::vector<int32_t> l1 = {1,2,5,5,6,10};
    std::vector<int32_t> l2 = {2,5,8,10,20};
    
    std::vector<int32_t> res;
    res = merge(l1, l2);
    
    for(size_t i = 0; i < res.size(); ++i){
        std::cout << res[i] << ' ';
    }
    
    std::cout << std::endl;
    
    return 0;
}