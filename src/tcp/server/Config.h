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

#pragma once

#include <sniper/http/server/Config.h>
#include <sniper/std/filesystem.h>
#include <sniper/std/string.h>

namespace sniper::tcp_test::server {

class Config final
{
public:
    explicit Config(const fs::path& config);

    [[nodiscard]] const http::server::Config& http_server_config() const noexcept;

    [[nodiscard]] const string& ip() const noexcept;
    [[nodiscard]] uint16_t port() const noexcept;
    [[nodiscard]] unsigned threads_count() const noexcept;
    [[nodiscard]] unsigned response_size() const noexcept;

private:
    void load_config(const fs::path& p);

    http::server::Config _http_server_config;

    string _ip;
    unsigned _port = 8080;
    unsigned _threads_count = 1;
    unsigned _response_size = 4096;
};

} // namespace sniper::tcp_test
