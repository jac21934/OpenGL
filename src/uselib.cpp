#include "uselib.h"

std::string getFileContents(const char* filename){
    std::ifstream in;
    in.open(filename, std::ios::in);
    if(in.is_open()){
        std::string fileContents;
        in.seekg(0, std::ios::end);
        fileContents.resize(in.tellg());
        in.seekg(0,std::ios::beg);
        in.read(&fileContents[0], fileContents.size());
        in.close();
        return(fileContents);
    }
    else{
        throw(errno);
    }
}


std::string getFileNameExtension(std::string fileName){
    
    size_t i = fileName.rfind('.', fileName.length());
    if (i != std::string::npos) {
        return(fileName.substr(i+1, fileName.length() - i));
    }
    
    return("");
    
}
