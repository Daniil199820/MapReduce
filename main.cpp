#include <filesystem>
#include "Map_reduce.h"
#include <map>
#include <vector>
#include <math.h>
/**
 * В этом файле находится клиентский код, который использует наш MapReduce фреймворк.
 * Этот код знает о том, какую задачу мы решаем.
 * Задача этого кода - верно написать мапер, редьюсер, запустить mapreduce задачу, обработать результат.
 * Задача - найти минимальную длину префикса, который позволяет однозначно идентифицировать строку в файле.
 * Задача не решается в одну mapreduce задачу. Нужно делать несколько запусков.
 * 
 * Как предлагаю делать я:
 * Выделяем первые буквы слов (в мапере), решаем для них задачу "определить, есть ли в них повторы".
 * Если не прокатило, повторяем процедуру, выделяя первые две буквы.
 * И т.д. В итоге найдём длину префикса, который однозначно определяет строку.
 * 
 * Здесь описано то, как я примерно решал бы задачу, это не руководство к действию, а просто пояснение к основному тексту задания.
 * Вы можете поступать по-своему (не как я описываю), задание творческое!
 * Можете делать так, как написано, если считаете, что это хорошо.
 * 
 */

std::vector<int> merge(const std::vector<int>& left_arr,const std::vector<int>& right_arr){
    int l = 0;
    int r = 0;
    std::vector<int> result_arr;
    
    while(l<left_arr.size() && r<right_arr.size()){
        result_arr.push_back((left_arr[l]<right_arr[r])?left_arr[l++]:right_arr[r++]);
    }

    while(l<left_arr.size()){
        result_arr.push_back(left_arr[l++]);
    }

    while(r<right_arr.size()){
        result_arr.push_back(right_arr[r++]);
    }

    return result_arr;

}

std::vector<int> mergeSort(std::vector<int>& vec){
    if(vec.size()==1){
        return vec;
    }
    int med = std::ceil(vec.size()/2);
    std::vector<int> left_arr;
    std::vector<int> right_arr;
    
    for(int i=0; i< med;++i){
        left_arr.push_back(vec[i]);
    }

    for(int i = med;i<vec.size();++i){
        right_arr.push_back(vec[i]);
    }

    return merge(mergeSort(left_arr),mergeSort(right_arr));
} 


int main(int argc, char* argv[]) {


    MapReduce mp;
    std::filesystem::path input("test.txt");
    std::filesystem::path output("./out/");
    std::function<void(std::string& line)> ff = [](std::string& line){
        line = line + " " +std::to_string(1);
    };


   

    mp.set_mapper(ff);
    mp.run(input);
    //std::vector<std::thread> vect;

    //auto fd = [](){std::cout<<"print\n";};

    //std::function<void()> f = std::move(fd);

    //vect.emplace_back(f);

    //for(auto& cur_thread:vect){
    //    cur_thread.join();
    //}

   // std::filesystem::path input("emails.txt");
   // std::filesystem::path output("./out/");
   // int mappers_count = 3;
   // int reducers_count = 2;

  /*  MapReduce mr(mappers_count, reducers_count);

    цикл по длине префикса {
        mr.set_mapper([](){
            // моё предложение: 
            //     * получает строку, 
            //     * выделяет префикс, 
            //     * возвращает пары (префикс, 1).
        });
        mr.set_reducer([](){
            // моё предложение: 
            //     * получает пару (префикс, число),
            //     * если текущий префикс совпадает с предыдущим или имеет число > 1, то возвращает false,
            //     * иначе возвращает true.
            //
            // Почему тут написано "число", а не "1"? 
            // Чтобы учесть возможность добавления фазы combine на выходе мапера.
            // Почитайте, что такое фаза combine в hadoop.
            // Попробуйте это реализовать, если останется время.
        });
        mr.run(input, output);
    }
 */
    return 0;
}