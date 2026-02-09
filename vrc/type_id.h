#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <string_view>
#include <type_traits>

#define VRC_DECLARE_CLASS_NAME(ty) \
    template<> \
    struct util::class_name<ty> { \
        static constexpr auto value{util::make_class_name(#ty)}; \
    }

// NOLINTBEGIN(readability-identifier-naming*)
namespace util {

template<typename Ty>
struct class_name;

template<std::size_t N>
constexpr std::array<char, N - 1> make_class_name(const char (&name)[N]) {
    std::array<char, N - 1> name_array;
    for (std::size_t i = 0; i < N - 1; ++i) { name_array[i] = name[i]; }
    return name_array;
}

class type_id {
 private:
    std::uint64_t id_ = 0;

 public:
    constexpr type_id() = default;
    explicit constexpr type_id(std::uint64_t id) : id_(id) {}

    std::uint64_t value() const { return id_; }

    bool operator==(const type_id& type) const { return id_ == type.id_; }
    bool operator!=(const type_id& type) const { return id_ != type.id_; }
};

constexpr std::uint64_t calc_hash64(const char* data, std::size_t size) {
    // Implementation of MurmurHash64A (used in GNU libstdc++ for std::hash<std::string>)
    constexpr std::uint64_t seed = 0xc70f6907;
    constexpr std::uint64_t mul = 0xc6a4a7935bd1e995ULL;
    constexpr auto shift_mix = [](std::uint64_t v) { return v ^ (v >> 47); };
    const char* p8 = data;
    std::uint64_t hash = seed ^ (static_cast<std::uint64_t>(size) * mul);
    for (const char* end8 = data + (size & ~7); p8 != end8; p8 += 8) {
        std::uint64_t a = 0;
        for (const char* p = p8 + 8; p != p8; --p) { a = (a << 8) + static_cast<std::uint8_t>(*(p - 1)); }
        hash = (hash ^ shift_mix(a * mul) * mul) * mul;
    }
    if (size & 7) {
        std::uint64_t a = 0;
        for (const char* p = data + size; p != p8; --p) { a = (a << 8) + static_cast<std::uint8_t>(*(p - 1)); }
        hash = (hash ^ a) * mul;
    }
    return shift_mix(shift_mix(hash) * mul);
}

template<typename Ty>
struct class_id {
    static constexpr type_id value{calc_hash64(class_name<Ty>::value.data(), class_name<Ty>::value.size())};
};

template<typename Ty>
struct type_traits {
    using type = std::remove_cv_t<Ty>;
    static type_id id() { return class_id<type>::value; }
    static std::string_view name() { return {class_name<type>::value.data(), class_name<type>::value.size()}; }
};

}  // namespace util
// NOLINTEND(readability-identifier-naming*)

template<>
struct std::hash<util::type_id> {
    std::size_t operator()(util::type_id k) const { return std::hash<std::uint64_t>{}(k.value()); }
};
