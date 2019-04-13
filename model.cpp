/**
 * Author: Katarina Gresova, xgreso00
 * Datum: 12.04.2019
 * Name: model.cpp
 * Description: Model using differences.
 */

#include "model.hpp"

/**
 * Transforms input character according to model
 * @param input     input character
 * @return          transformed character
 */
u_int8_t modeling(u_int8_t input) {
    static u_int8_t previous_input = 0;
    u_int8_t difference = input - previous_input;
    previous_input = input;

    return difference;
}

/**
 * Transforms modeled character back to original character
 * @param difference    difference used as model
 * @return              original character
 */
u_int8_t demodeling(u_int8_t difference) {
    static u_int8_t previous_input = 0;
    u_int8_t output = previous_input + difference;
    previous_input = output;

    return output;
}
