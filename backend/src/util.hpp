/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <iomanip>
#include <stdexcept>
#include <variant>

#include <libtorrent/info_hash.hpp>

namespace util {

class BadResultAccess : public std::runtime_error {
public:
    explicit BadResultAccess(char const* const message) noexcept : std::runtime_error(message) {}
    
    char const* what() const noexcept override {
        return exception::what();
    }
};

template<class T, class E>
class Result {
private:
    enum class ResultType {
        ERROR,
        OK,
    };

    std::variant<T, E> m_value;
    ResultType m_resultType;

    explicit Result(ResultType resultType) : m_resultType(resultType) {};
public:
    static Result Ok(T value) {
        Result result(ResultType::OK);
        result.m_value = value;
        return result;
    }

    static Result Err(E error) {
        Result result(ResultType::ERROR);
        result.m_value = error;
        return result;
    }

    bool ok() const {
        return m_resultType == ResultType::OK;
    }

    T value() const {
        if (m_resultType != ResultType::OK) {
            throw BadResultAccess("can't get value of error type");
        }
        return std::get<T>(m_value);
    }

    E error() const {
        if (m_resultType != ResultType::ERROR) {
            throw BadResultAccess("can't get error of ok value");
        }
        return std::get<E>(m_value);
    }
};

template<std::ptrdiff_t N>
std::string hashToHex(lt::digest32<N> hash) {
    std::stringstream hexStream;
    
    for (auto byte = hash.begin(); byte != hash.end(); ++byte) {
        int byteAsInt = static_cast<int>(*byte);
        hexStream << std::setfill('0') << std::setw(2) << std::hex << byteAsInt;
    }

    return hexStream.str();
}

template<std::ptrdiff_t N>
lt::digest32<N> hexToHash(std::string& hex) {
    if (hex.length() * 4 != N) {
        throw std::invalid_argument("hex string is wrong length");
    }

    const int numBytes = hex.length() / 2U;
    
    char buffer[numBytes];

    for (int i = 0; i < numBytes; i++) {
        std::stringstream hexByteStream(hex.substr(2 * i, 2));
        int byte;
        hexByteStream >> std::hex >> byte;
        buffer[i] = static_cast<char>(byte);
    }

    return lt::digest32<N>(buffer);
}

}
