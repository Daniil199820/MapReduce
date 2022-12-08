#include <filesystem>
#include "Map_reduce.h"
#include <map>
#include <vector>
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

void print(){
    std::cout<<"awd\n";
}

struct S{
    S(){
        std::cout<<"Cntr\n";
    }

    S (const S& s) noexcept{
        std::cout<< "Copy cntr\n";
    }

    S(S&& s) noexcept{
        std::cout<< "Move cntr\n";
    }


};

int main(int argc, char* argv[]) {


    MapReduce mp;
    std::filesystem::path input("test.txt");
    std::filesystem::path output("./out/");
    std::function<void(std::string& line)> ff = [](std::string& line){
        line = line + " " +std::to_string(1);
    };

    std::function<int(std::map<const int, int> k)> rf = [](std::map<const int, int> k){
        std::cout<<k[0]<<"";
        return 0;
    };

    std::vector<S> s_vec;

    S s1;
    S s2;
    S s3;

    s_vec.emplace_back(std::move(s1));
    s_vec.emplace_back(std::move(s2));
    s_vec.emplace_back(std::move(s3));

    mp.set_mapper(ff);
    //mp.set_mapper_2(rf);
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