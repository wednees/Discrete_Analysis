#include <vector>
#include <string>

struct Person
{
    uint32_t age;
    std::string name;
};

// Необходимо отсортировать список людей по их возрасту

std::vector<Person> sort_ages(const std::vector<Person>& ages)
{
    
    short int max = 0;
    for(size_t i = 0; i < ages.size(); ++i){
        if(ages[i].age > max){
            max = ages[i].age;
        }
    }
    
    std::vector<int> tmp(max, 0);
    
    for (const Person& person : ages) {
        tmp[person.age]++;
    }
    
    for (size_t i = 1; i < tmp.size(); ++i){
        tmp[i] = tmp[i] + tmp[i-1];
    }
    
    std::vector<Person> res(ages.size());
    for (int i = ages.size() - 1; i >= 0; --i){
        int idx = tmp[ages[i].age];
        tmp[ages[i].age]--;
        res[idx - 1] = ages[i];
    }
    
    
    return res;
}
