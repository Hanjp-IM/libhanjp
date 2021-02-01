#include "hanjp.h"

using namespace Hanjp;
using namespace std;

u16string InputContext::flush_internal() {
    return {};
}

u16string InputContext::flush() {
    return {};
}

AMSIG InputContext::process(int ascii) {
    return {};
}

bool InputContext::backspace() {
    return {};
}

void InputContext::set_output_type(OutputType type) {
    output_type = type;
}

const u16string& InputContext::get_preedit_string() const {
    return preedit;
}

const u16string& InputContext::get_commit_string() const {
    return committed;
}

OutputType InputContext::get_output_type() const {
    return output_type;
}