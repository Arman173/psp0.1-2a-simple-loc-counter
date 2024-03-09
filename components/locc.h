/****************************************************************
 * Program Assigment: Read LOC component.
 * Author: Armando Haziel Canul Cen
 * Date: 05/03/2024
 * Description: Component that contain the tools for read C++ LOC.
 ****************************************************************/

#ifndef _LOCC_HPP
#define _LOCC_HPP

#include <iostream>
#include <fstream>
#include <cstring>

#define LOC 0
#define NO_LOC 1
#define COMPLEX_COMMENT 2

class loc_file {
    private:
        std::string file_name;
        int loc;
    public:
        loc_file(std::string);
        std::string get_file_name();
        int getLOC();
        bool countLOC();
};

bool is_cpp_file(std::string);
bool is_bracket_symbol(char);
/* declaraciones de las funciones de la maquina
 * de estados para la comprobacion de la terminacion
 * de un comentarion de varias lineas (COMPLEX COMMENT).*/
bool is_complex_comment_end(const char*);
bool m1(const char*);
/* declaraciones de las funciones de la maquina
 * de estados para la comprobacion lineas de codigo
 * para verificar si lo son para contar.*/
int check_cpp_line(const char*);
int q1(const char*);
int q2(const char*);
int q3(const char*);
int q4(const char*);
int q5(const char*);

#endif