#include <cstring>
#include "debug_api.h"


static char* GetFileNameOnly(const char* name)
{
    if(!name) return nullptr;
    std::string temp_name(name);
    std::string name_only;
    std::string::size_type pos = temp_name.find_last_of('\\');
    if(pos != std::string::npos)
        name_only = temp_name.substr(pos+1);
    else
    {
        pos = temp_name.find_last_of('/');
        if(pos != std::string::npos)
            name_only = temp_name.substr(pos+1);
        else
            name_only.assign(name);
    }

    size_t length = name_only.length();
    char* str_nameonly = new char [length+1];
    if(!str_nameonly) return nullptr;
    std::strcpy(str_nameonly, name_only.c_str());
    return str_nameonly;
}

void ShowIndexErrorMessage(unsigned int range, unsigned int index, const char* file_name, const char* function_name, int line_number)
{
    char *file_name_only = GetFileNameOnly(file_name);
    std::cout<<"In File :"<<file_name_only<<", Function: "<<function_name<<", Line: "<<line_number<<",\n";
    std::cout<<"Index Range is "<<range<<", Current index is "<<index<<".";
    std::cout<<std::endl;
    delete [] file_name_only;
}

void ShowMemoryErrorMessage(const char* file_name, const char* function_name, int line_number)
{
    char *file_name_only = GetFileNameOnly(file_name);
    std::cout<<"In File :"<<file_name_only<<", Function: "<<function_name<<", Line: "<<line_number<<",\n";
    std::cout<<"Allocate Memory Error.";
    std::cout<<std::endl;
    delete [] file_name_only;
}

IndexOverRangeError::IndexOverRangeError(unsigned int index_range, unsigned int current_index, const char* file_name,const char* function_name, const int line_number)
{
    m_function_name = function_name;
    m_index_range = index_range;
    m_current_index = current_index;
    m_file_name = GetFileNameOnly(file_name);
    m_line_number = line_number;
}

IndexOverRangeError::~IndexOverRangeError()
{ 
    if(m_file_name) 
    {
        delete [] m_file_name; 
        m_file_name = nullptr; 
    }
}

void IndexOverRangeError::show_message(std::ostream& os)
{
    os  <<"In source file: "<<m_file_name<<", ";
    os  <<"at line :  "<<m_line_number<<", \n";
    os  <<"when call function "<<m_function_name<<" commit an index out of range error, \n";
    os  <<"the index range is "<<m_index_range<< ", but ";
    os  <<"current index is "<<m_current_index <<" ." <<std::endl;
}

MemoryAllocatedError::MemoryAllocatedError(const char* file_name,const char* function_name, const int line_number)
{
    m_function_name = function_name;
    m_file_name = GetFileNameOnly(file_name);
    m_line_number = line_number;
}

MemoryAllocatedError::~MemoryAllocatedError()
{ 
    if(m_file_name) 
    {
        delete [] m_file_name; 
        m_file_name = nullptr; 
    }
}

void MemoryAllocatedError::show_message(std::ostream& os)
{
    os  <<"In source file: "<< m_file_name<<", ";
    os  <<"at line : "<<m_line_number<<", \n";
    os  <<"when call function "<<m_function_name<<" commit a memory allocated error. \n";
    os  <<std::endl;
}