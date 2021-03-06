/*******************************************************************************
* Copyright 2014-2020 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#pragma once

#include "oneapi/dal/data/common_helpers.hpp"
#include "oneapi/dal/data/table_metadata.hpp"

namespace dal::backend {

class homogen_table_impl {
public:
    template <typename DataType>
    homogen_table_impl(std::int64_t N, std::int64_t p, const DataType* data_pointer, data_layout layout)
        : row_count_(N),
          column_count_(p),
          finfo_(feature_info{ make_data_type<DataType>() }),
          meta_(table_metadata{ p, finfo_, layout }) {
        data_.reset_not_owning(reinterpret_cast<const byte_t*>(data_pointer),
                               N * p * sizeof(DataType));
    }

    template <typename DataType, typename = std::enable_if_t<!std::is_pointer_v<DataType>>>
    homogen_table_impl(std::int64_t N, std::int64_t p, DataType value, data_layout layout)
        : homogen_table_impl(N, p, fill_data(new DataType[N*p], N*p, value), layout) {}

    template <typename DataType>
    homogen_table_impl(std::int64_t p, const array<DataType>& data, data_layout layout)
        : row_count_(data.get_size() / p),
          column_count_(p),
          finfo_(feature_info{ make_data_type<DataType>() }),
          meta_(table_metadata{ p, finfo_, layout }) {

        if (row_count_ * column_count_ != data.get_size()) {
            throw std::runtime_error("data size must be power of column count");
        }

        const std::int64_t size_in_bytes = data.get_size() * sizeof(DataType);
        if (data.is_data_owner() && data.has_mutable_data()) {
            data_.reset(reinterpret_cast<byte_t*>(data.get_mutable_data()),
                        size_in_bytes,
                        [owner = array(data)](auto) mutable { owner.reset(); });
        } else if (data.has_mutable_data()) {
            data_.reset_not_owning(reinterpret_cast<byte_t*>(data.get_mutable_data()),
                                   size_in_bytes);
        } else {
            // TODO: the case when data.is_data_owner() == true && data.has_mutable_data() == false
            // is impossible now, but can appear
            data_.reset_not_owning(reinterpret_cast<const byte_t*>(data.get_data()),
                                   size_in_bytes);
        }
    }

    std::int64_t get_column_count() const {
        return column_count_;
    }

    std::int64_t get_row_count() const {
        return row_count_;
    }

    const table_metadata& get_metadata() const {
        return meta_;
    }

    const void* get_data() const {
        return data_.get_data();
    }

    template <typename T>
    void pull_rows(array<T>& a, const range& r) const;

    template <typename T>
    void push_back_rows(const array<T>& a, const range& r);

    template <typename T>
    void pull_column(array<T>& a, std::int64_t idx, const range& r) const;

    template <typename T>
    void push_back_column(const array<T>& a, std::int64_t idx, const range& r);

private:
    template <typename T>
    static T* fill_data(T* data, std::int64_t size, const T& value) {
        for (std::int64_t i = 0; i < size; i++) {
            data[i] = value;
        }

        return data;
    }

private:
    std::int64_t row_count_;
    std::int64_t column_count_;
    feature_info finfo_;
    table_metadata meta_;
    array<byte_t> data_;
};

} // namespace dal::backend
