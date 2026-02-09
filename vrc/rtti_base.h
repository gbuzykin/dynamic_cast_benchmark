#pragma once

#include "type_id.h"

#include <memory>
#include <utility>

#define VRC_IMPLEMENT_DYNAMIC_CAST(...) \
    const void* dynamicCast(util::type_id id) const override { \
        using Ty = std::remove_const_t<std::remove_pointer_t<decltype(this)>>; \
        if (id == util::type_traits<Ty>::id()) { return this; } \
        return vrc::detail::dynamicCastImpl<Ty, ##__VA_ARGS__>{}(this, id); \
    } \
    void* dynamicCast(util::type_id id) { return const_cast<void*>(std::as_const(*this).dynamicCast(id)); } \
    static_assert(true)

namespace vrc {

struct RttiBase {
    virtual ~RttiBase() = default;
    virtual const void* dynamicCast(util::type_id id) const = 0;
    void* dynamicCast(util::type_id id) { return const_cast<void*>(std::as_const(*this).dynamicCast(id)); }
};

template<typename Ty>
struct AsIface {
    using type = Ty;
};

template<typename Ty2, typename Ty, typename = std::enable_if_t<std::is_convertible_v<Ty*, Ty2*>>>
Ty2* upCast(Ty* ptr) {
    return ptr;
}

template<typename Ty2, typename Ty, typename = std::enable_if_t<std::is_convertible_v<Ty*, Ty2*>>>
std::shared_ptr<Ty2> upCast(const std::shared_ptr<Ty>& ptr) {
    return std::shared_ptr<Ty2>{ptr};
}

template<typename Ty2, typename Ty, typename = std::enable_if_t<std::is_convertible_v<Ty*, Ty2*>>>
std::shared_ptr<Ty2> upCast(std::shared_ptr<Ty>&& ptr) {
    return std::shared_ptr<Ty2>{std::move(ptr)};
}

template<typename Ty2, typename Ty>
Ty2* dynamicCast(Ty* ptr) {
    if constexpr (std::is_convertible_v<Ty*, Ty2*>) { return ptr; }
    return ptr ? static_cast<Ty2*>(ptr->dynamicCast(util::type_traits<Ty2>::id())) : nullptr;
}

template<typename Ty2, typename Ty>
std::shared_ptr<Ty2> dynamicCast(const std::shared_ptr<Ty>& ptr) {
    if (auto* result = dynamicCast<Ty2>(ptr.get())) { return {ptr, result}; }
    return nullptr;
}

template<typename Ty2, typename Ty>
std::shared_ptr<Ty2> dynamicCast(std::shared_ptr<Ty>&& ptr) {
    if (auto* result = dynamicCast<Ty2>(ptr.get())) { return {std::move(ptr), result}; }
    return nullptr;
}

template<typename Ty2, typename Ty>
bool isKindOf(Ty* ptr) {
    if constexpr (std::is_convertible_v<Ty*, Ty2*>) { return true; }
    return ptr && ptr->dynamicCast(util::type_traits<Ty2>::id());
}

template<typename Ty2, typename Ty>
bool isKindOf(const std::shared_ptr<Ty>& ptr) {
    return isKindOf<Ty2>(ptr.get());
}

namespace detail {
template<typename, typename...>
struct dynamicCastImpl {};
template<typename Ty>
struct dynamicCastImpl<Ty> {
    const void* operator()(const Ty*, util::type_id) const { return nullptr; }
};
template<typename Ty, typename Ty2, typename... Tail>
struct dynamicCastImpl<Ty, AsIface<Ty2>, Tail...> {
    const void* operator()(const Ty* ptr, util::type_id id) const {
        if (id == util::type_traits<Ty2>::id()) { return static_cast<const Ty2*>(ptr); }
        return dynamicCastImpl<Ty, Tail...>{}(ptr, id);
    }
};
template<typename Ty, typename Ty2, typename... Tail>
struct dynamicCastImpl<Ty, Ty2, Tail...> {
    const void* operator()(const Ty* ptr, util::type_id id) const {
        if (const void* result = dynamicCastImpl<Ty, Tail...>{}(ptr, id)) { return result; }
        return ptr->Ty2::dynamicCast(id);
    }
};
}  // namespace detail

}  // namespace vrc
