/**
@mainpage CS 361 - Homework 7
@section Description
This program simulates threaded search program which take 2 inputs, the target and folder needed. the program will search through the folder and find if there is a match in the folder's and its subfolder's file. 

*/
/**
 * @file main.cpp
 * @author 
 *   Justin Nguyen <jn867@drexel.edu>
 * @date November 24, 2025
 *
 * @section DESCRIPTION
 * This file contains a search program that scans through files in a directory and looks for a target string.
 */
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <filesystem>
#include <unordered_set>
#include <fstream>

#include "channel.h"
// info struct to hold file path, line number, and thread id
typedef struct info{
    std::string filepath;// file path of the matched line
    int lineNumber;// line number of the matched line
    std::thread::id id;// thread id that found the match
} info;

/**
    The Producer thread recursively scans a directory, filters files by extension, and sends each valid file path to the workers through a buffered channel.
 @param folder       The root directory where the search begins.
 @param task_channel The channel used to send file paths to worker threads.
 @param print_channel The channel used to send info structs to printer thread.
 @param num_workers  The number of worker threads (used for sending poison pills).
*/

void Producer(const std::string& folder, 
              channel<std::string>& task_channel,
                channel<info>& print_channel,
              int num_workers);
/**
 Each Worker thread receives file paths from the channel, opens the file, and searches for occurrences of a target string.
If the match is found, the thread print it.     
    @param task_channel The channel used to send file paths to worker threads.
    @param print_channel The channel used to send info structs to printer thread.
    @param target The target string we need to find. 
*/
void Worker(channel<std::string>& task_channel,
            channel<info>& print_channel,
            const std::string& target);
/**
prints out the info struct received from worker threads.
    @param print_channel The channel used to send info structs to printer thread
    @param num_workers The number of worker threads (used for receiving poison pills).
 
*/
void print(channel<info>& print_channel, int num_workers);
/**
    Main function run the Mandelbrot set image generator with a thread manager and thread workers.
    @param argc Argument count
    @param argv Argument vector
    @return 0 for sucess and failure
*/

int main(int argc, char* argv[])
{
    if (argc != 2 && argc != 3) {
        std::cout << "Invalid input: bin/search [target string] [directory]\n";
        return 0;
    }
    std::string folder;
    std::string target = argv[1];
    if (argc == 2) folder ="." ;
    else folder = argv[2];

    int num_thread = std::thread::hardware_concurrency();
    if (num_thread < 2) num_thread = 2;

    std::cout << "!________Search Started________!\n";
    std::cout << "Target Folder:  " << folder << "\n";
    std::cout << "Target text:  " << target << "\n";
    std::cout << "Using " << num_thread << " threads\n";

    channel<std::string>* task_channel = makeChannel<std::string>(num_thread);
    channel<info>* print_channel = makeChannel<info>(num_thread);
    std::thread producer( Producer, folder, std::ref(*task_channel), std::ref(*print_channel), num_thread);
    std:: thread print_thread(print, std::ref(*print_channel), num_thread);
    std::vector<std::thread> thread_vector;
    int poison_numeber =0;
    for (int i = 0; i < num_thread; i++) {
        thread_vector.emplace_back(Worker, std::ref(*task_channel), std::ref(*print_channel), std::ref(target));
    }

    for (int i = 0; i < num_thread; i++) {
        thread_vector[i].join();
    }

    producer.join();
    print_thread.join();
    delete task_channel;
    delete print_channel;
    std::cout << "!________Search Completed________!\n";
    return 0;
}

void Producer(const std::string& folder, 
              channel<std::string>& task_channel,
              channel<info>& print_channel,
              int num_workers)
{
    std::unordered_set<std::string> allowed_ext = {
        ".c", ".cc", ".cpp", ".h", ".hpp", ".py", ".pl", ".sh", ".txt"
    };
    /* Start of Cited Code
    Describe Reference (website, person, ect): website
    Provide a link for any website: https://en.cppreference.com/w/cpp/filesystem/recursive_directory_iterator.html
    */
    namespace fs = std::filesystem;
    for (auto& entry : fs::recursive_directory_iterator(folder)) {
        std::string ext = entry.path().extension().string();
    /* End of Cited Code */
        if (allowed_ext.count(ext)) {
            task_channel.send(entry.path().string());
        }
    }
    for (int i = 0; i < num_workers; i++) {
        task_channel.send("__POISON__");//signal that the search is done. 
    }
    
} 
void Worker(channel<std::string>& task_channel, channel<info>& print_channel,
            const std::string& target)
{
    while (true) {
        std::string filepath = task_channel.receive();

        if (filepath == "__POISON__") {
            print_channel.send(info{"__POISON__", -1, std::thread::id{}});
            return;
        }

        std::ifstream fin(filepath);
        if (!fin.is_open()) {
            continue;
        }

        std::string line;
        int lineNumber = 1;

        while (std::getline(fin, line)) {
            if(line.find(target) != std::string::npos) {
                info data;
                data.filepath = filepath;
                data.lineNumber = lineNumber;
                data.id = std::this_thread::get_id();
                print_channel.send(data);
            }
            lineNumber++;
        }
    }
}
void print(channel<info>& print_channel, int num_workers)
{   int poison_count =0;
    while (true) {
        info data = print_channel.receive();
        if (data.filepath == "__POISON__") {
            
            poison_count++;
            if (poison_count == num_workers) {
                return;
            }
            continue;
        }
        std::ifstream fin(data.filepath);
        if (!fin.is_open()) {
            continue;
        }
        std::string line;
        for (int i = 1; i <= data.lineNumber; i++) {
            std::getline(fin, line);
        }
        std::cout << "_________________________________\n";
        std::cout << "Thread " << data.id<< " found a match.\n";
        std::cout << "File: \"" << data.filepath << "\"\n";
        std::cout << "Line "    << data.lineNumber << ": " << line << "\n";
        std::cout << "_________________________________\n";

        
    }
}
