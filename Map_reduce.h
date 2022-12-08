/**
 * Это MapReduce фреймворк.
 * Он универсальный.
 * Он может выполнять разные map_reduce задачи.
 * Он просто обрабатывает какие-то данные с помощью каких-то функций в нескольких потоках.
 * Он ничего не знает о задаче, которую решает.
 * Здесь не должно быть кода, завязанного на конкретную задачу - определение длины префикса.
 * 
 * С помощью этого фреймворка должны решаться разные задачи.
 * Когда напишете это, попробуйте решить с помощью этого фреймворка все задачи, которые мы разбирали на лекции.
 * 
 * Это наш самописный аналог hadoop mapreduce.
 * Он на самом деле не работает с по-настоящему большими данными, потому что выполняется на одной машине.
 * Но мы делаем вид, что данных много, и представляем, что наши потоки - это процессы на разных узлах.
 * 
 * Ни один из потоков не должен полностью загружать свои данные в память или пробегаться по всем данным.
 * Каждый из потоков обрабатывает только свой блок.
 * 
 * На самом деле даже один блок данных не должен полностью грузиться в оперативку, а должен обрабатываться построчно.
 * Но в домашней работе можем этим пренебречь и загрузить один блок в память одним потоком.
 * 
 * Всё в этом файле - это рекомендация.
 * Если что-то будет слишком сложно реализовать, идите на компромисс, пренебрегайте чем-нибудь.
 * Лучше сделать что-нибудь, чем застрять на каком-нибудь моменте и не сделать ничего.
 */

#include <iostream>
#include <filesystem>
#include <vector>
#include <functional>
#include <stdio.h>
#include <fstream>
#include <thread>
#include <future>
#include <sstream>

std::string read_block(const std::filesystem::path& filename,size_t begin,size_t end){
    std::string str(end-begin,'\0');
    std::ifstream file(filename);
    file.seekg(begin,std::ios::beg);
    for(int i = 0;i<end-begin;++i){
        str[i] = file.get();
    }
    return str;
}

class MapReduce {
public:
    void run(const std::filesystem::path& input) {
       auto blocks = split_file(input, mappers_count);
        // set mapper
         for(int j = 0; j<mappers_count;++j){
            mapper_threads.emplace_back([&,j](){
                std::stringstream fl_name;
                fl_name << "file_"<<std::this_thread::get_id();
                std::ofstream f_out(fl_name.str());
                std::stringstream temp_fl(read_block(input,blocks[j].from,blocks[j].to));
                std::string line;
                std::vector<std::string> string_vector; 
                while(std::getline(temp_fl,line)){
                    string_vector.push_back(std::move(line));   
                }
                std::sort(string_vector.begin(),string_vector.end());
                for(auto & it: string_vector){
                    mapper(it);
                    f_out << it <<"\n";
                }
                f_out.close();
            });
        }

        for(auto& cur_thread:mapper_threads){
            if(cur_thread.joinable()){
                cur_thread.join();
            }
        }

        for(int j = 0; j<reducers_count;++j){
            reducer_threads.emplace_back([&,j](){
                std::stringstream fl_name;
                fl_name << "file_"<<std::this_thread::get_id();
                std::ofstream f_out(fl_name.str());
                std::stringstream temp_fl(read_block(input,blocks[j].from,blocks[j].to));
                std::string line;
                std::vector<std::string> string_vector; 
                while(std::getline(temp_fl,line)){
                    string_vector.push_back(std::move(line));   
                }
                std::sort(string_vector.begin(),string_vector.end());
                for(auto & it: string_vector){
                    mapper(it);
                    f_out << it <<"\n";
                }
                f_out.close();
            });
        }

        for(auto& cur_thread:reducer_threads){
            if(cur_thread.joinable()){
                cur_thread.join();
            }
        }  

        // Создаём mappers_count потоков
        // В каждом потоке читаем свой блок данных
        // Применяем к строкам данных функцию mapper
        // Сортируем результат каждого потока
        // Результат сохраняется в файловую систему (представляем, что это большие данные)
        // Каждый поток сохраняет результат в свой файл (представляем, что потоки выполняются на разных узлах)


        // Создаём reducers_count новых файлов
        // Из mappers_count файлов читаем данные (результат фазы map) и перекладываем в reducers_count (вход фазы reduce)
        // Перекладываем так, чтобы:
        //     * данные были отсортированы
        //     * одинаковые ключи оказывались в одном файле, чтобы одинаковые ключи попали на один редьюсер
        //     * файлы примерно одинакового размера, чтобы редьюсеры были загружены примерно равномерно
        //
        // Гуглить: алгоритмы во внешней памяти, external sorting, многопутевое слияние
        //
        // Для упрощения задачи делаем это в один поток
        // Но все данные в память одновременно не загружаем, читаем построчно и пишем
        //
        // Задание творческое!
        // Я не уверен, что все вышеперечисленные требования выполнимы одновременно
        // Возможно, придётся идти на компромисс, упрощая какие-то детали реализации
        // Но это то, к чему нужно стремиться
        // Проектирование ПО часто требует идти на компромиссы
        // Это как оптимизация функции многих переменных с доп. ограничениями


        // Создаём reducers_count потоков
        // В каждом потоке читаем свой файл (выход предыдущей фазы)
        // Применяем к строкам функцию reducer
        // Результат сохраняется в файловую систему 
        //             (во многих задачах выход редьюсера - большие данные, хотя в нашей задаче можно написать функцию reduce так, чтобы выход не был большим)
    }

    void set_mapper(std::function<void(std::string&)> func){
        mapper = func;
    }

    //void set_mapper_3()

    template<typename T, typename U>
    void set_mapper_2(std::function<T(U)> f){
        mapper = f;
    }

    
private:
    struct Block {

        size_t from;
        size_t to;
    };

    std::vector<std::thread> mapper_threads;
    std::vector<std::thread> reducer_threads;

    std::vector<Block> split_file(const std::filesystem::path& file, int blocks_count) {
        
        std::vector<Block> function_result;
        int byte_size = std::filesystem::file_size(file);

        int num_pages = byte_size/blocks_count;
        //std::cout<< byte_size<<"\n";
        size_t counter = 0;
        while (counter <byte_size){
            Block block;
            block.from = counter;
            counter += num_pages;
            if(counter>byte_size)counter = byte_size-1;
            //std::cout<<counter<<"\n";
            block.to = find_EOL(file,counter,byte_size);
            ++counter;
            function_result.emplace_back(block);
        } 

        return function_result;
        /**
         * Эта функция не читает весь файл.
         * 
         * Определяем размер файла в байтах.
         * Делим размер на количество блоков - получаем границы блоков.
         * Читаем данные только вблизи границ.
         * Выравниваем границы блоков по границам строк.
         */
    }

    int find_EOL(const std::filesystem::path& file, size_t& counter,size_t max_counter){

        std::ifstream myfile;
        myfile.open(file);
        if(myfile.is_open()){
            char symbol;
            int i_left = counter;
            int i_right = counter;
            myfile.seekg(counter,std::ios::beg);
            symbol = myfile.get();
          //  std::cout<<symbol<<"\n";
           bool flag_r_l = true;
           while(symbol != '\n' && i_left>0 && i_right<max_counter){
                if(flag_r_l){
                    myfile.seekg(++i_right,std::ios::beg);
                    symbol = myfile.get();
                    //std::cout << symbol;
                    flag_r_l = false;
               //     std::cout<<"false\n";
                }
                else{
                    myfile.seekg(--i_left,std::ios::beg);
                    symbol = myfile.get();
                    //std::cout << symbol;
                    flag_r_l = true;
             //       std::cout<<"true\n";
                }
           }
           if(flag_r_l){
            counter = i_left;
            //std::cout<<counter;
            //std::cout<<symbol<<"\n";
            return i_left;
           }
           else{
            counter = i_right;
            //std::cout<<counter;
            //std::cout<<symbol<<"\n";
            return i_right;
           }
        }
        myfile.close();
        return 0;
    }

    int mappers_count = 1 ;
    int reducers_count = 3;
    //std::ifstream f_in;
   // template<typename ...Args>
   std::function<void(std::string& )> mapper;
   std::future<void> mapper2;
   // std::function</*type*/> reducer;
};