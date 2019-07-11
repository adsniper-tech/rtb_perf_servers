// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
 * Copyright (c) 2019, AdSniper, Oleg Romanenko (oleg@romanenko.ro)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <libconfig.h++>
#include <sniper/log/log.h>
#include <sniper/std/check.h>
#include <sniper/std/filesystem.h>
#include <thread>
#include "Config.h"

namespace sniper::udp_test::server {

Config::Config(const fs::path& config_p) : _threads_count(std::thread::hardware_concurrency())
{
    load_config(config_p);
}

void Config::load_config(const fs::path& p)
{
    {
        error_code ec;
        check(fs::exists(p, ec) && !ec, "Config file {} does not exists", p.string());
        check(fs::file_size(p, ec) && !ec, "Empty config file");
    }

    log_info("Load config file: {}", p.string());

    libconfig::Config cfg;
    cfg.readFile(p.c_str());

    if (cfg.getRoot().exists("core")) {
        auto& core = cfg.getRoot()["core"];

        core.lookupValue("threads", _threads_count);
        if (!_threads_count)
            _threads_count = std::thread::hardware_concurrency();

        core.lookupValue("ip", _ip);
        core.lookupValue("port", _port);
    }

    if (cfg.getRoot().exists("udp_test")) {
        auto& test = cfg.getRoot()["udp_test"];

        test.lookupValue("response_size", _response_size);
    }

    // Print setings
    log_info("{}, build {}, {} UTC, rev {}", APP_NAME, __DATE__, __TIME__, GIT_SHA1);
    log_info("Config dump:");
    log_info("core");
    log_info("\tip: {}:{}", _ip.empty() ? "0.0.0.0" : _ip, _port);
    log_info("\tthreads: {}", _threads_count);

    log_info("udp_test");
    log_info("\tresponse_size: {}", _response_size);
}

uint16_t Config::port() const noexcept
{
    return _port;
}

unsigned Config::threads_count() const noexcept
{
    return _threads_count;
}
const string& Config::ip() const noexcept
{
    return _ip;
}

unsigned Config::response_size() const noexcept
{
    return _response_size;
}

} // namespace sniper::udp_test::server
