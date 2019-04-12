//
// Created by Katarina Gresova on 12/04/2019.
//

#include "model.hpp"

u_int8_t modeling(u_int8_t input) {
    static u_int8_t previous_input = 0;
    u_int8_t difference = input - previous_input;
    previous_input = input;

    return difference;
}

u_int8_t demodeling(u_int8_t difference) {
    static u_int8_t previous_input = 0;
    u_int8_t output = previous_input + difference;
    previous_input = output;

    return output;
}
