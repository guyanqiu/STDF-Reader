#ifndef _DEBUG_API_H_
#define _DEBUG_API_H_
#include <iostream>
#include <fstream>

#ifndef __FUNCDNAME__
#define __FUNCDNAME__ __func__
#endif
#define INDEX_OVER_RANGE(range, index) IndexOverRangeError(range, index, __FILE__, __FUNCDNAME__, __LINE__)
#define MEMORY_ALLOCATED_ERROR()       MemoryAllocatedError(__FILE__, __FUNCDNAME__, __LINE__)
#define SHOW_INDEX_ERROR(range, index) ShowIndexErrorMessage(range, index, __FILE__, __FUNCDNAME__, __LINE__)
#define SHOW_MEMORY_ERROR()            ShowMemoryErrorMessage(__FILE__, __FUNCDNAME__, __LINE__)


void ShowIndexErrorMessage(unsigned int range, unsigned int index, const char* file_name, const char* function_name, int line_number);
void ShowMemoryErrorMessage(const char* file_name, const char* function_name, int line_number);

class DebugMessage
{
public:
    virtual void show_message(std::ostream& os){ os<<"Error."<<std::endl; }
    DebugMessage(){};
    virtual ~DebugMessage(){};
};

class IndexOverRangeError : public DebugMessage
{
private:
    const char* m_function_name;
    unsigned int m_index_range, m_current_index;
    char* m_file_name;
    int m_line_number;
public:
    IndexOverRangeError(unsigned int index_range, unsigned int current_index, 
    const char* file_name,const char* function_name, const int line_number);
    virtual ~IndexOverRangeError();
    virtual void show_message(std::ostream& os);
};


class MemoryAllocatedError  : public DebugMessage
{
private:
    const char* m_function_name;
    unsigned int m_index_range, m_current_index;
    char* m_file_name;
    int m_line_number;

public:
    MemoryAllocatedError(const char* file_name,const char* function_name, const int line_number);
    virtual ~MemoryAllocatedError();
    virtual void show_message(std::ostream& os);
};



#endif//_DEBUG_API_H_