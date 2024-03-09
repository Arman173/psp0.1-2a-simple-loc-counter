/****************************************************************
 * Program Assigment: Program 2a, LOC COUNTER.
 * Author: Armando Haziel Canul Cen
 * Date: 05/03/2024
 * Description: Program that count LOC from file or directory.
 ****************************************************************/

#include <iostream>
#include <fstream>
#include <dirent.h>
#include <queue>
#include "components/locc.h"
using namespace std;

// function to save all c++ files paths from directory in a queue.
void get_all_cpp_files_from_dir(string, queue<string>&);
// function to save all loc data in a file.
void save_data(queue<loc_file>&);

int main(int argc, const char *argv[]) {

    queue<string> directories;
    string input;

    // si no hay argumentos, se le pedira el archivo/directorio al usuario.
    if (argc > 1) {
        input = argv[1];
    } else {
        cout << "Introduzca el directorio a leer las LOC: ";
        cin >> input;
    }

    cout << endl;
    cout << "----------------------------------------------------------------" << endl;
    cout << "Adding..." << endl;
    if (is_cpp_file(input)) {
        cout << "added: " << input << endl;
        directories.push(input);
    } else {
        get_all_cpp_files_from_dir(input, directories);
    }
    cout << endl;

    cout << endl;
    cout << "----------------------------------------------------------------" << endl;
    cout << "Reading LOC..." << endl;
    queue<loc_file> files;
    int totalLOC = 0;
    while (!directories.empty()) {
        string fpath = directories.front(); // file path from directories queue
        directories.pop();
        loc_file file(fpath);
        if (file.countLOC()) {
            cout << file.get_file_name() << ": " << file.getLOC() << endl;
            totalLOC += file.getLOC();
            files.push(file);
        } else {
            cout << "Error open the file: " << file.get_file_name() << endl;
        }
    }
    cout << endl;
    cout << "TOTAL LOC from " << files.size() << " files: ";
    cout << totalLOC << endl;

    char d; // decision
    cout << endl;
    cout << "----------------------------------------------------------------" << endl;
    cout << "Desea guardar los datos [y/n]: ";
    cin >> d;

    if (d == 'y' || d == 'Y') {
        save_data(files);
    }

    return 0;
}

void get_all_cpp_files_from_dir(string directory, queue<string> &queue_to_save) {
    DIR* dir = opendir(directory.c_str());
    if (dir == nullptr) {
        cerr << "Error trying open the directory: " << directory << endl;
        return;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        string entry_name(entry->d_name);
        // don't do nothing if the directory is the root(.) or prev of root(..)
        if (entry_name == "." || entry_name == "..")
            continue;
        if (entry->d_type == DT_DIR) {
            string subdir = directory + "/" + entry->d_name;
            get_all_cpp_files_from_dir(subdir, queue_to_save);
        } else if (entry->d_type == DT_REG) {
            if (is_cpp_file(entry_name)) {
                string file_path = directory + "/" + entry->d_name;
                queue_to_save.push(file_path);
                cout << "added: " << file_path << endl;
            }
        }
    }
    closedir(dir);
}

void save_data(queue<loc_file> &data) {
    string directory;
    string name;
    cout << "Introduzca el directorio donde se guardara el archivo (debe terminar en /):" << endl;
    cin >> directory;
    cout << "Introduzca el nombre del archivo: ";
    cin >> name;
    string antDir = directory; // copia de el directorio para usarla despues.
    directory = directory + name + ".txt";
    ofstream file(directory);
    if (!file.is_open()) {
        cout << "Error al abrir el archivo..." << endl;
    }
    /* Escribimos en el archivo */
    file << " ------------------------------------------------------\n";
    file << "  Directory: " << directory << "\n";
    file << "  Language: C++\t\t\t\t" << data.size() << " files\n";
    file << " ------------------------------------------------------\n";
    file << "  LOC\tdirectory file\n";
    int total_loc = 0;
    while (!data.empty()) {
        loc_file current = data.front();
        file << " " << current.getLOC() << " \t" << current.get_file_name() << "\n";
        total_loc += current.getLOC();
        data.pop();
    }
    file << " ------------------------------------------------------\n";
    file << "  " << total_loc << " from: " << antDir;
    /* Terminamos de escribir en el archivo */
    file.close();
    cout << "Informacion guardada correctamente en:" << endl;
    cout << directory << endl;
}