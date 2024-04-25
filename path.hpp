#ifndef _path_hpp
#define _path_hpp

#include <vector>

class Path
{
public:
    Path(unsigned **weights_matrix);

    /**
     * @brief Print the path
     *
     */
    void print();

    /**
     * @brief Add a city to the path
     *
     * @param city City to add
     * @return new path weight
     */
    unsigned add(unsigned city);

    /**
     * @brief Get the path weight
     *
     * @return unsigned
     */
    unsigned get_weight();

    /**
     * @brief Get the path
     *
     * @return std::vector<unsigned>
     */
    std::vector<unsigned> get_path();

private:
    unsigned path_weight;
    std::vector<unsigned> current_path;
    unsigned **weights_matrix;
};

#endif //_path_hpp
