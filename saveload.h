#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <typeinfo>

// Declaration

template <typename T>
void Serialize(T pod, std::ostream& out);

template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out);

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out);

template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data);


template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data);

// Serialization

template <typename T>
void Serialize(T pod, std::ostream& out) {
    out.write(reinterpret_cast<const char*>(&pod), sizeof(pod));
}

void Serialize(const std::string& str, std::ostream& out) {
    Serialize(str.size(), out);
    out.write(str.c_str(), str.size());
}

template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out) {
    Serialize(data.size(), out);
    for (auto& obj : data) {
        Serialize(obj, out);
    }
}

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out) {
    Serialize(data.size(), out);
    for (const auto& [key, val] : data) {
        Serialize(key, out);
        Serialize(val, out);
    }
}

// Deserialization

template <typename T>
void Deserialize(std::istream& in, T& pod) {
    in.read(reinterpret_cast<char*>(&pod), sizeof(pod));
}

void Deserialize(std::istream& in, std::string& str) {
    size_t sz = 0;
    Deserialize(in, sz);
    str.resize(sz);
    in.read(str.data(), sz);
}

template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data) {
    size_t sz = 0;
    Deserialize(in, sz);
    data.resize(sz);
    for (int i = 0; i < sz; ++i) {
        T obj;
        Deserialize(in, obj);
        data[i] = std::move(obj);
    }
}

template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data) {
    size_t sz = 0;
    Deserialize(in, sz);
    for (int i = 0; i < sz; ++i) {
        T1 key;
        T2 val;
        Deserialize(in, key);
        Deserialize(in, val);
        data[std::move(key)] = std::move(val);
    }
}
