#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>


using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

struct Pair{
    unsigned first;
    std::string second;
};

namespace sort{
    void counting_sort(std::vector<Pair> &elems){
        
        if(elems.empty()){
            return;
        }

        unsigned short max_elem = 0;
        for(size_t i = 0; i < elems.size(); ++i){
            max_elem = (elems[i].first > max_elem) ? elems[i].first : max_elem;
        }

        // vector::Vector<int> tmp(max_elem + 1);
        int tmp[max_elem + 1] = {0};
        // for(size_t i = 0; i < tmp.size(); ++i){
        //     tmp[i] = 0;
        // }

        for(size_t i = 0; i < elems.size(); ++i){
            ++tmp[elems[i].first];
        }

        for (size_t i = 1; i < max_elem + 1; ++i){
            tmp[i] += tmp[i - 1];
        }

        std::vector<Pair> res(elems.size());
        // size_t pos;
        for(int i = elems.size() - 1; i >= 0; --i){
            //unsigned short key = elems[i].first;
            size_t pos = tmp[elems[i].first]--;
            res[pos-1] = elems[i];
        }

        // std::swap(elems, res);
        elems = res;
    }

}

bool comp(Pair a, Pair b){
    return a.first < b.first;
}

int main()
{
    std::vector<Pair> input, input_stl;
    std::ifstream is("test_mil.txt");
    // std::ofstream os("output_mil.txt");
    Pair elem;
    while (is >> elem.first >> elem.second)
    {
        input.push_back(elem);
    }
    input_stl = input;

    std::cout << "Count of lines is " << input.size() << std::endl;

    // Измеряем время работы сортировки подсчётом.
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
    sort::counting_sort( input );
    auto end_ts = std::chrono::system_clock::now();
    uint64_t counting_sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();

    // Измеряем время работы stl сортировки.
    start_ts = std::chrono::system_clock::now();
    std::stable_sort(std::begin(input_stl), std::end(input_stl), comp);
    end_ts = std::chrono::system_clock::now();

    uint64_t stl_sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    std::cout << "Counting sort time: " << counting_sort_ts << DURATION_PREFIX << std::endl;
    std::cout << "STL stable sort time: " << stl_sort_ts << DURATION_PREFIX << std::endl;
}