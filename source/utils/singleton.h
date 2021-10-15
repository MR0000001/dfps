/*
Dfps
Copyright (C) 2021 Matt Yang(yccy@outlook.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <memory>
#include <mutex>

template <typename T>
class Singleton {
public:
    Singleton() = default;
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;

    static T *GetInstance(void);

private:
    static T *instance_;
    static std::once_flag once_;
};

#define DECLARE_SINGLETON(T)                                                                                           \
    template <typename T>                                                                                              \
    T *Singleton<T>::instance_ = nullptr;                                                                              \
    template <typename T>                                                                                              \
    std::once_flag Singleton<T>::once_;                                                                                \
    template <typename T>                                                                                              \
    T *Singleton<T>::GetInstance(void) {                                                                               \
        std::call_once(once_, []() { instance_ = new T(); });                                                          \
        return instance_;                                                                                              \
    }