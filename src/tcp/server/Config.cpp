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

namespace sniper::tcp_test::server {

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

    if (cfg.getRoot().exists("tcp_test")) {
        auto& test = cfg.getRoot()["tcp_test"];

        test.lookupValue("response_size", _response_size);
    }

    if (cfg.getRoot().exists("http")) {
        auto& http = cfg.getRoot()["http"];

        if (http.exists("server")) {
            auto& server = http["server"];

            if (unsigned int num; server.lookupValue("max_conns", num))
                _http_server_config.max_conns = num;

            server.lookupValue("backlog", _http_server_config.backlog);

            if (unsigned num; server.lookupValue("conns_clean_interval_seconds", num))
                _http_server_config.conns_clean_interval = seconds(num);

            if (unsigned num; server.lookupValue("keep_alive_timeout_seconds", num))
                _http_server_config.connection.keep_alive_timeout = seconds(num);

            if (unsigned num; server.lookupValue("request_read_timeout_seconds", num))
                _http_server_config.connection.request_read_timeout = seconds(num);

            if (unsigned num; server.lookupValue("request_body_max_size", num))
                _http_server_config.connection.message.body_max_size = num;
        }
    }

    // Print setings
    log_info("{}, build {}, {} UTC, rev {}", APP_NAME, __DATE__, __TIME__, GIT_SHA1);
    log_info("Config dump:");
    log_info("core");
    log_info("\tip: {}:{}", _ip.empty() ? "0.0.0.0" : _ip, _port);
    log_info("\tthreads: {}", _threads_count);

    log_info("tcp_test");
    log_info("\tresponse_size: {}", _response_size);

    log_info("http server");
    log_info("\tmax_conns: {}", _http_server_config.max_conns);
    log_info("\tbacklog: {}", _http_server_config.backlog);
    log_info("\tconns_clean_interval_seconds: {}", _http_server_config.conns_clean_interval.count());
    log_info("\tkeep_alive_timeout_seconds: {}",
             duration_cast<seconds>(_http_server_config.connection.keep_alive_timeout).count());
    log_info("\trequest_read_timeout_seconds: {}",
             duration_cast<seconds>(_http_server_config.connection.request_read_timeout).count());
    log_info("\trequest_body_max_size: {}", _http_server_config.connection.message.body_max_size);
}

const http::server::Config& Config::http_server_config() const noexcept
{
    return _http_server_config;
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

} // namespace sniper::tcp_test::server
