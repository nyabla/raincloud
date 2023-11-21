/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <stdexcept>

namespace util {

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
        Result result(esultType::ERROR);
        result.m_error = error;
        return result;
    }

    bool ok() {
        return m_resultType == ResultType::OK;
    }

    T value() {
        if (m_resultType != ResultType::OK) {
            throw std::runtime_error("can't get value of error type");
        }
        return m_value.get<T>();
    }

    E error() {
        if (m_resultType != ResultType::ERROR) {
            throw std::runtime_error("can't get error of ok value");
        }
        return m_value.get<E>();
    }
};

}
