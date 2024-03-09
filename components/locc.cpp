#include "locc.h"
using namespace std;

const string EXT[] = {"h", "hpp", "cpp"};

/**************************************************************
 *          CLASS FUNCTION DEFINITIONS
 **************************************************************/
loc_file::loc_file(string file_name) {
    this->file_name = file_name;
    this->loc = -1;
}
string loc_file::get_file_name() {
    return this->file_name;
}
int loc_file::getLOC() {
    return this->loc;
}
bool loc_file::countLOC() {
    ifstream file(this->file_name);
    if (!file.is_open())
        return false;
    string line;
    int loc = 0;
    bool complex_comment = false;

    while (getline(file, line)) {
        if (line.empty())
            continue;
        const char* str = line.c_str();
        if (complex_comment) {
            if (!is_complex_comment_end(str))
                continue;
            loc++;
            // cout << "/*     COMPLEX COMMENT     */" << endl;
            complex_comment = false;
        } else {
            int status = check_cpp_line(str);
            // if (status == LOC)
            //     cout << line << endl;
            if (status == LOC)
                loc++;
            else if (status == COMPLEX_COMMENT)
                complex_comment = true;
            else
                continue;
        }
    }
    file.close();
    this->loc = loc;
    return true;
}

/**************************************************************
 *          FUNCTION DEFINITIONS
 **************************************************************/

bool is_cpp_file(string file_name) {
    char* str = &file_name[0]; // pointer to filename string
    char* p; // previous string pointer
    char* c; // current string pointer
    c = strtok(str, ".");
    while (c != NULL) {
        p = c;
        c = strtok(NULL, ".");
    }
    string ext(p);
    for (int i = 0; i < 3; i++)
        if (ext == EXT[i])
            return true;
    // file_name is not a c++ file
    return false;
}

bool is_bracket_symbol(char c) {
    return (c=='{' || c=='}' || c=='[' || c==']' || c=='(' || c==')');
}

bool is_complex_comment_end(const char* c) {
    if (*c == '\0')
        return false;
    else if (*c == '*')
        return m1(++c);
    else
        return is_complex_comment_end(++c);
}
bool m1(const char* c) {
    if (*c == '\0')
        return false;
    else if (*c == '*')
        return m1(++c);
    else if (*c == '/')
        return true;
    else
        return is_complex_comment_end(++c);
}

int check_cpp_line(const char* c) {
    if (*c == '\0')
        return NO_LOC;
    else if (*c == ' ')
        return check_cpp_line(++c);
    else if (*c == '/')
        return q1(++c);
    else if (is_bracket_symbol(*c))
        return q2(++c);
    else
        return LOC;
}
int q1(const char* c) {
    if (*c == '\0')
        return LOC;
    else if (*c == '/')
        return NO_LOC;
    else if (*c == '*')
        return q3(++c);
    else
        return LOC;
}
int q2(const char* c) {
    if (*c == '\0')
        return NO_LOC;
    else if (*c == ' ' || *c == ';' || is_bracket_symbol(*c))
        return q2(++c);
    else if (*c == '/')
        return q1(++c);
    else
        return LOC;
}
int q3(const char* c) {
    if (*c == '\0')
        return COMPLEX_COMMENT;
    else if (*c == '*')
        return q4(++c);
    else
        return q3(++c);
}
int q4(const char* c) {
    if (*c == '\0')
        return COMPLEX_COMMENT;
    else if (*c == '*')
        return q4(++c);
    else if (*c == '/')
        return q5(++c);
    else
        return q3(++c);
}
int q5(const char* c) {
    if (*c == '\0')
        return NO_LOC;
    else if (*c == ' ' || *c == ';' || is_bracket_symbol(*c))
        return q5(++c);
    else if (*c == '/')
        return q1(++c);
    else
        return LOC;
}