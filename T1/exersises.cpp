#include <iostream>
#include <deque>
#include <vector>
#include <string>
#include "log_duration.h"
//#include "random_varvbar.h"

#include <string_view>

const std::size_t container_size = 500;

std::vector<std::string_view> SplitIntoWordsView(const std::string& str){
    std::vector<std::string_view> result;
    // 1
    auto pos = str.find_first_not_of(" ");
    // 2
    const auto pos_end = str.npos;
    // 3
    while (pos != pos_end) {
        // 4
        auto space = str.find(' ', pos);
        // 5
        result.push_back(space == pos_end ? str.substr(pos) : str.substr(pos, space - pos));
        // 6
        pos = str.find_first_not_of(" ", space);
    }

    return result;
} 

int main () {

using namespace std;

const string text = "a b";
    /* {
        LOG_DURATION("string");
        const auto words = SplitIntoWords(text);
        cout << words[0] << "\n";
    } */
    {
        LOG_DURATION("string_view");
        const auto words = SplitIntoWordsView(text);
        cout << words[0] << "\n";
    }



   /*  {
        LOG_DURATION("vector testing");
        
        std::vector<int> test_vector;

        for (size_t i = 0; i < container_size; ++i){

            int element = static_cast<int> (arc4random());
            test_vector.push_back(element);
        }

        std::sort(test_vector.begin(), test_vector.end());
    } */
    
    //std::cout << std::endl;

    /* {
        LOG_DURATION("deq testing");

        std::deque<int> test_deq;

        for (size_t i = 0; i < container_size; ++i){

            int element = static_cast<int> (arc4random());
            test_deq.push_back(element);
        }

        std::sort(test_deq.begin(), test_deq.end());
    } */

    /* {
        using namespace std;
        deque<int> numbers = {1};
        auto it = numbers.begin();
        cout << *it << endl;

        for (int i = 0; i < 1024; ++i) {
            numbers.push_back(i);
        }
        // попытаемся вывести последний элемент из дека
        cout << *it << " " << *(it + numbers.size() - 1) << endl;
    }  */
   
    //std::cout << std::endl;

    return 0;

}