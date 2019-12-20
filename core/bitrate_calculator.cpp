/*
The MIT License (MIT)

Copyright (c) 2016-2018 RabbitStreamer

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "bitrate_calculator.h"
#include "logger.h"
#include <sys/time.h>

namespace rs {
namespace core {

RsBitrateCalculator::RsBitrateCalculator(string name) {
    calculator_name = name;
    recv_buffer_size = 0;
    last_recv_msec = 0;
}

RsBitrateCalculator::~RsBitrateCalculator() {}

void RsBitrateCalculator::update_buffersize(uint32_t size) {
    timeval tv;
    recv_buffer_size += size;
    if (gettimeofday(&tv, NULL) == -1) {
        return;
    }

    uint32_t curr_ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    uint32_t delta_ms = curr_ms - last_recv_msec;
    if (delta_ms > 1000) {
        RSLOGI("%s bitrate:%d kbps", calculator_name.c_str(),
               1000 * recv_buffer_size / (1024 * delta_ms));
        recv_buffer_size = 0;
        last_recv_msec = curr_ms;
    }
}

} // namespace core
} // namespace rs