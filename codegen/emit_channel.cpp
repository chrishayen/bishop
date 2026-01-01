/**
 * @file emit_channel.cpp
 * @brief Channel emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Emits a channel creation using bishop::rt::Channel.
 */
string emit_channel_create(CodeGenState& state, const ChannelCreate& channel) {
    string cpp_type = map_type(channel.element_type);

    if (channel.capacity) {
        string cap = emit(state, *channel.capacity);
        return "bishop::rt::Channel<" + cpp_type + ">(" + cap + ")";
    }

    return "bishop::rt::Channel<" + cpp_type + ">()";
}

} // namespace codegen
