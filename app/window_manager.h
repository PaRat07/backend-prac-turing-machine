#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <future>

#include "window.h"
#include "general_data.h"



class WindowManager {
public:
    void AddWindow(Window win) {
        windows_.push_back(std::move(win));
    }

    void Start();

private:
    size_t active_ = 0;
    std::vector<Window> windows_;
};
