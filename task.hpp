#ifndef _task_hpp
#define _task_hpp

#include <vector>

#include "path.hpp"

class Task
{
public:
    Task();

private:
    Path current_path;
    std::vector<unsigned> cites_left;
};

#endif //_task_hpp
