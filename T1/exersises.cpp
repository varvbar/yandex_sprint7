#include <iostream>
#include <deque>
#include <vector>
#include "log_duration.h"
//#include "random_varvbar.h"

const std::size_t container_size = 500;

int main () {


    {
        LOG_DURATION("vector testing");
        
        std::vector<int> test_vector;

        for (size_t i = 0; i < container_size; ++i){

            int element = static_cast<int> (arc4random());
            test_vector.push_back(element);
        }

        std::sort(test_vector.begin(), test_vector.end());
    }
    
    std::cout << std::endl;

    {
        LOG_DURATION("deq testing");

        std::deque<int> test_deq;

        for (size_t i = 0; i < container_size; ++i){

            int element = static_cast<int> (arc4random());
            test_deq.push_back(element);
        }

        std::sort(test_deq.begin(), test_deq.end());
    }

    {
        using namespace std;
        deque<int> numbers = {1};
        auto it = numbers.begin();
        cout << *it << endl;

        for (int i = 0; i < 1024; ++i) {
            numbers.push_back(i);
        }
        // попытаемся вывести последний элемент из дека
        cout << *it << " " << *(it + numbers.size() - 1) << endl;
    } 
   
    std::cout << std::endl;

    return 0;
   
}