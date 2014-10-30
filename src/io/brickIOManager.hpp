#ifndef _BRICKIOMANAGER_HPP
#define _BRICKIOMANAGER_HPP

#include "../mainApplication.hpp"
#include <string>

class BrickIOManager {
    public:
    virtual void load(MainApplication *app, std::string filenm) = 0;
    virtual void save(MainApplication *app, std::string filenm) = 0;
};

#endif
