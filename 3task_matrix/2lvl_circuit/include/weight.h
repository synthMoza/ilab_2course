#pragma once

#include <iostream>

namespace se {
    // Contains resistance and voltage of the circuit's edge
    struct Weight {
        double resistance_;
        double voltage_;

        Weight(double resistance = 0, double voltage = 0) :
            resistance_ (resistance), voltage_ (voltage) {}
        
        bool is_zero() {
            return (resistance_ == 0 && voltage_ == 0);
        }
    };
}