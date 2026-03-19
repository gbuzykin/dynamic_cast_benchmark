/*
 * Program listing for my blog post "Performance comparison of three different
 * implementations of dynamic_cast" [1]
 *
 * [1]: https://blog.michael.franzl.name/2021/03/21/performance-comparison-of-three-different-implementations-of-dynamic_cast/
 *
 * MIT License
 *
 * Copyright (c) 2021 Michael Karl Franzl
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include "vrc/rtti_base.h"
#include "KCL/KCL_RTTI.h"

#include <iostream>
#include <chrono>
#include <vector>
#include <memory>
#include <cstdlib>
#include <typeinfo>
#include <string>
#include <functional>
#include <random>
#include <algorithm>

enum class Hierarchy { deep, shallow, balanced, cross };
enum class SortOrder { aligned, shuffled };

double max_num_ops = 0;

constexpr uint64_t N = 2'000'000; // number of iterations
const int USECS_PER_SEC = 1'000'000;

struct A : KCL::RTTI::Base, vrc::RttiBase { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(); uint64_t a{1}; uint64_t z{0}; };
KCL_RTTI_REGISTER(A);
VRC_DECLARE_CLASS_NAME(A);

namespace deep {
    struct B : A { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(A); uint64_t b{1}; };
    struct C : B { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(B); uint64_t c{1}; };
    struct D : C { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(C); uint64_t d{1}; };
    struct E : D { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(D); uint64_t e{1}; };
    struct F : E { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(E); uint64_t f{1}; };
    struct G : F { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(F); uint64_t g{1}; };
    struct H : G { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(G); uint64_t h{1}; };
}
KCL_RTTI_REGISTER(deep::B, A);
KCL_RTTI_REGISTER(deep::C, deep::B);
KCL_RTTI_REGISTER(deep::D, deep::C);
KCL_RTTI_REGISTER(deep::E, deep::D);
KCL_RTTI_REGISTER(deep::F, deep::E);
KCL_RTTI_REGISTER(deep::G, deep::F);
KCL_RTTI_REGISTER(deep::H, deep::G);

VRC_DECLARE_CLASS_NAME(deep::B);
VRC_DECLARE_CLASS_NAME(deep::C);
VRC_DECLARE_CLASS_NAME(deep::D);
VRC_DECLARE_CLASS_NAME(deep::E);
VRC_DECLARE_CLASS_NAME(deep::F);
VRC_DECLARE_CLASS_NAME(deep::G);
VRC_DECLARE_CLASS_NAME(deep::H);

namespace shallow {
    struct B : A { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(A); uint64_t b{1}; };
    struct C : A { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(A); uint64_t c{1}; };
    struct D : A { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(A); uint64_t d{1}; };
    struct E : A { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(A); uint64_t e{1}; };
    struct F : A { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(A); uint64_t f{1}; };
    struct G : A { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(A); uint64_t g{1}; };
    struct H : A { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(A); uint64_t h{1}; };
}
KCL_RTTI_REGISTER(shallow::B, A);
KCL_RTTI_REGISTER(shallow::C, shallow::B);
KCL_RTTI_REGISTER(shallow::D, shallow::C);
KCL_RTTI_REGISTER(shallow::E, shallow::D);
KCL_RTTI_REGISTER(shallow::F, shallow::E);
KCL_RTTI_REGISTER(shallow::G, shallow::F);
KCL_RTTI_REGISTER(shallow::H, shallow::G);

VRC_DECLARE_CLASS_NAME(shallow::B);
VRC_DECLARE_CLASS_NAME(shallow::C);
VRC_DECLARE_CLASS_NAME(shallow::D);
VRC_DECLARE_CLASS_NAME(shallow::E);
VRC_DECLARE_CLASS_NAME(shallow::F);
VRC_DECLARE_CLASS_NAME(shallow::G);
VRC_DECLARE_CLASS_NAME(shallow::H);

namespace balanced {
    struct B : A { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(A); uint64_t b{1}; };
    struct C : B { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(B); uint64_t c{1}; };
    struct D : B { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(B); uint64_t d{1}; };

    struct E : A { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(A); uint64_t e{1}; };
    struct F : E { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(E); uint64_t f{1}; };
    struct G : E { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(E); uint64_t g{1}; };
    struct H : E { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(E); uint64_t h{1}; };
}
KCL_RTTI_REGISTER(balanced::B, A);
KCL_RTTI_REGISTER(balanced::C, balanced::B);
KCL_RTTI_REGISTER(balanced::D, balanced::B);
KCL_RTTI_REGISTER(balanced::E, A);
KCL_RTTI_REGISTER(balanced::F, balanced::E);
KCL_RTTI_REGISTER(balanced::G, balanced::E);
KCL_RTTI_REGISTER(balanced::H, balanced::E);

VRC_DECLARE_CLASS_NAME(balanced::B);
VRC_DECLARE_CLASS_NAME(balanced::C);
VRC_DECLARE_CLASS_NAME(balanced::D);
VRC_DECLARE_CLASS_NAME(balanced::E);
VRC_DECLARE_CLASS_NAME(balanced::F);
VRC_DECLARE_CLASS_NAME(balanced::G);
VRC_DECLARE_CLASS_NAME(balanced::H);

namespace cross {
    struct B : KCL::RTTI::Base, vrc::RttiBase { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(); uint64_t b{1}; };
    struct C : KCL::RTTI::Base, vrc::RttiBase { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(); uint64_t c{1}; };
    struct D : KCL::RTTI::Base, vrc::RttiBase { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(); uint64_t d{1}; };
    struct E : KCL::RTTI::Base, vrc::RttiBase { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(); uint64_t e{1}; };
    struct F : A, B { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(vrc::AsIface<A>, vrc::AsIface<B>); uint64_t f{1}; };
    struct G : F, C, D { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(F, vrc::AsIface<C>, vrc::AsIface<D>); uint64_t g{1}; };
    struct H : G, E { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(G, vrc::AsIface<E>); uint64_t h{1}; };
}
KCL_RTTI_REGISTER(cross::B);
KCL_RTTI_REGISTER(cross::C);
KCL_RTTI_REGISTER(cross::D);
KCL_RTTI_REGISTER(cross::E);
KCL_RTTI_REGISTER(cross::F, A, cross::B);
KCL_RTTI_REGISTER(cross::G, cross::F, cross::C, cross::D);
KCL_RTTI_REGISTER(cross::H, cross::G, cross::E);

VRC_DECLARE_CLASS_NAME(cross::B);
VRC_DECLARE_CLASS_NAME(cross::C);
VRC_DECLARE_CLASS_NAME(cross::D);
VRC_DECLARE_CLASS_NAME(cross::E);
VRC_DECLARE_CLASS_NAME(cross::F);
VRC_DECLARE_CLASS_NAME(cross::G);
VRC_DECLARE_CLASS_NAME(cross::H);

// Same interface as A, but not related.
struct Z : KCL::RTTI::Base, vrc::RttiBase { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(); uint64_t z{1}; };
KCL_RTTI_REGISTER(Z);
VRC_DECLARE_CLASS_NAME(Z);


struct JustKclRtti : KCL::RTTI::Base { KCL_RTTI_IMPL(); };
KCL_RTTI_REGISTER(JustKclRtti);

struct JustVrcRtti : vrc::RttiBase { VRC_IMPLEMENT_DYNAMIC_CAST(); };
VRC_DECLARE_CLASS_NAME(JustVrcRtti);

struct JustRtti : KCL::RTTI::Base, vrc::RttiBase { KCL_RTTI_IMPL(); VRC_IMPLEMENT_DYNAMIC_CAST(); };
KCL_RTTI_REGISTER(JustRtti);
VRC_DECLARE_CLASS_NAME(JustRtti);

double dummy = 0;
std::default_random_engine rng{};

void draw_bar(double percent, std::string s = "-") {
    const int cols = 80;
    uint64_t width = uint64_t(cols * percent);
    if (width > cols) {
        width = cols;
        printf("|");
        while (width--) std::cout << s;
        printf("...\n");
    } else {
        printf("|");
        while (width--) std::cout << s;
        printf("|\n");
    }
}

double run(std::string label, std::function<uint64_t()> benchmark)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    uint64_t successes = benchmark();
    auto t2 = std::chrono::high_resolution_clock::now();

    auto usecs_total =
        std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();
    double num_ops = USECS_PER_SEC / (double(usecs_total) / N);
    if (max_num_ops == 0) { max_num_ops = num_ops; } // the first run will be 100%
    double percent = num_ops / max_num_ops;
    printf(
            "%3s: %6.1f MHz (%3.0f%%) [%7llu] ",
            label.c_str(),
            num_ops / USECS_PER_SEC,
            percent * 100, static_cast<unsigned long long>(successes)
          );
    draw_bar(percent);
    return num_ops;
}

std::vector<std::unique_ptr<A>> generate_data(Hierarchy h, unsigned from, unsigned width)
{
    std::vector<std::unique_ptr<A>> v;
    v.reserve(N);

    std::uniform_int_distribution<unsigned> distrib(from, from + width);

    for(uint64_t i = 0; i < N; ++i) {
        unsigned val = distrib(rng);
        if (h == Hierarchy::deep) {
            switch(val) {
                case  0: v.emplace_back(std::make_unique<A>()); break;
                case  1: v.emplace_back(std::make_unique<deep::B>()); break;
                case  2: v.emplace_back(std::make_unique<deep::C>()); break;
                case  3: v.emplace_back(std::make_unique<deep::D>()); break;
                case  4: v.emplace_back(std::make_unique<deep::E>()); break;
                case  5: v.emplace_back(std::make_unique<deep::F>()); break;
                case  6: v.emplace_back(std::make_unique<deep::G>()); break;
                case  7: v.emplace_back(std::make_unique<deep::H>()); break;
            }
        } else if (h == Hierarchy::shallow) {
            switch(val) {
                case  0: v.emplace_back(std::make_unique<A>()); break;
                case  1: v.emplace_back(std::make_unique<shallow::B>()); break;
                case  2: v.emplace_back(std::make_unique<shallow::C>()); break;
                case  3: v.emplace_back(std::make_unique<shallow::D>()); break;
                case  4: v.emplace_back(std::make_unique<shallow::E>()); break;
                case  5: v.emplace_back(std::make_unique<shallow::F>()); break;
                case  6: v.emplace_back(std::make_unique<shallow::G>()); break;
                case  7: v.emplace_back(std::make_unique<shallow::H>()); break;
            }
        } else if (h == Hierarchy::balanced) {
            switch(val) {
                case  0: v.emplace_back(std::make_unique<A>()); break;
                case  1: v.emplace_back(std::make_unique<balanced::B>()); break;
                case  2: v.emplace_back(std::make_unique<balanced::C>()); break;
                case  3: v.emplace_back(std::make_unique<balanced::D>()); break;
                case  4: v.emplace_back(std::make_unique<balanced::E>()); break;
                case  5: v.emplace_back(std::make_unique<balanced::F>()); break;
                case  6: v.emplace_back(std::make_unique<balanced::G>()); break;
                case  7: v.emplace_back(std::make_unique<balanced::H>()); break;
            }
        } else if (h == Hierarchy::cross) {
            switch(val) {
                case  5: v.emplace_back(std::make_unique<cross::F>()); break;
                case  6: v.emplace_back(std::make_unique<cross::G>()); break;
                case  7: v.emplace_back(std::make_unique<cross::H>()); break;
            }
        }
    }
    return v;
}

void shuffle(std::vector<std::unique_ptr<A>>& v) {
    std::shuffle(std::begin(v), std::end(v), rng);
}

void print_average(double num) {
    double avg = num / 8.0;
    printf("------------\n");
    printf("AVG: %6.1f MHz                  ", avg / USECS_PER_SEC);
    draw_bar(avg / max_num_ops, "=");
}

void run_benchmarks(std::vector<std::unique_ptr<A>>& v, Hierarchy h)
{
    double sum = 0;

    max_num_ops = 0;

    // Cache warming
    dummy += [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = static_cast <      A*>(e.get()); s += p ? p->a : e->z; } return s; }();

    printf("Base-line: static_cast\n");
    printf("```\n");
             run("-", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = static_cast <      A*>(e.get()); s += p ? p->a : e->z; } return s; });
    printf("```\n\n");

    if (h == Hierarchy::deep) {
        printf("Implementation: `dynamic_cast`\n");
        printf("```\n");
        sum = 0;
        dummy += run("A", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<      A*>(e.get()); s += p ? p->a : e->z; } return s; });
        sum   += run("B", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<deep::B*>(e.get()); s += p ? p->b : e->z; } return s; });
        sum   += run("C", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<deep::C*>(e.get()); s += p ? p->c : e->z; } return s; });
        sum   += run("D", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<deep::D*>(e.get()); s += p ? p->d : e->z; } return s; });
        sum   += run("E", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<deep::E*>(e.get()); s += p ? p->e : e->z; } return s; });
        sum   += run("F", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<deep::F*>(e.get()); s += p ? p->f : e->z; } return s; });
        sum   += run("G", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<deep::G*>(e.get()); s += p ? p->g : e->z; } return s; });
        sum   += run("H", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<deep::H*>(e.get()); s += p ? p->h : e->z; } return s; });
        sum   += run("Z", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<      Z*>(e.get()); s += p ? p->z : e->z; } return s; });
        print_average(sum);
        printf("```\n");

        printf("Implementation: `vrc::dynamicCast`\n");
        printf("```\n");
        sum = 0;
        dummy += run("A", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<      A>(e.get()); s += p ? p->a : e->z; } return s; });
        sum   += run("B", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<deep::B>(e.get()); s += p ? p->b : e->z; } return s; });
        sum   += run("C", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<deep::C>(e.get()); s += p ? p->c : e->z; } return s; });
        sum   += run("D", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<deep::D>(e.get()); s += p ? p->d : e->z; } return s; });
        sum   += run("E", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<deep::E>(e.get()); s += p ? p->e : e->z; } return s; });
        sum   += run("F", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<deep::F>(e.get()); s += p ? p->f : e->z; } return s; });
        sum   += run("G", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<deep::G>(e.get()); s += p ? p->g : e->z; } return s; });
        sum   += run("H", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<deep::H>(e.get()); s += p ? p->h : e->z; } return s; });
        sum   += run("Z", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<      Z>(e.get()); s += p ? p->z : e->z; } return s; });
        print_average(sum);
        printf("```\n\n");

        printf("Implementation: `kcl_dynamic_cast`\n");
        printf("```\n");
        sum = 0;
        dummy += run("A", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<      A*>(e.get()); s += p ? p->a : e->z; } return s; });
        sum   += run("B", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<deep::B*>(e.get()); s += p ? p->b : e->z; } return s; });
        sum   += run("C", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<deep::C*>(e.get()); s += p ? p->c : e->z; } return s; });
        sum   += run("D", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<deep::D*>(e.get()); s += p ? p->d : e->z; } return s; });
        sum   += run("E", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<deep::E*>(e.get()); s += p ? p->e : e->z; } return s; });
        sum   += run("F", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<deep::F*>(e.get()); s += p ? p->f : e->z; } return s; });
        sum   += run("G", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<deep::G*>(e.get()); s += p ? p->g : e->z; } return s; });
        sum   += run("H", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<deep::H*>(e.get()); s += p ? p->h : e->z; } return s; });
        sum   += run("Z", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<      Z*>(e.get()); s += p ? p->z : e->z; } return s; });
        print_average(sum);
        printf("```\n\n");

    } else if (h == Hierarchy::shallow) {
        printf("Implementation: `dynamic_cast`\n");
        printf("```\n");
        sum = 0;
        dummy += run("A", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<         A*>(e.get()); s += p ? p->a : e->z; } return s; });
        sum   += run("B", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<shallow::B*>(e.get()); s += p ? p->b : e->z; } return s; });
        sum   += run("C", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<shallow::C*>(e.get()); s += p ? p->c : e->z; } return s; });
        sum   += run("D", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<shallow::D*>(e.get()); s += p ? p->d : e->z; } return s; });
        sum   += run("E", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<shallow::E*>(e.get()); s += p ? p->e : e->z; } return s; });
        sum   += run("F", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<shallow::F*>(e.get()); s += p ? p->f : e->z; } return s; });
        sum   += run("G", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<shallow::G*>(e.get()); s += p ? p->g : e->z; } return s; });
        sum   += run("H", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<shallow::H*>(e.get()); s += p ? p->h : e->z; } return s; });
        sum   += run("Z", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<         Z*>(e.get()); s += p ? p->z : e->z; } return s; });
        print_average(sum);
        printf("```\n\n");

        printf("Implementation: `vrc::dynamicCast`\n");
        printf("```\n");
        sum = 0;
        dummy += run("A", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<         A>(e.get()); s += p ? p->a : e->z; } return s; });
        sum   += run("B", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<shallow::B>(e.get()); s += p ? p->b : e->z; } return s; });
        sum   += run("C", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<shallow::C>(e.get()); s += p ? p->c : e->z; } return s; });
        sum   += run("D", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<shallow::D>(e.get()); s += p ? p->d : e->z; } return s; });
        sum   += run("E", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<shallow::E>(e.get()); s += p ? p->e : e->z; } return s; });
        sum   += run("F", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<shallow::F>(e.get()); s += p ? p->f : e->z; } return s; });
        sum   += run("G", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<shallow::G>(e.get()); s += p ? p->g : e->z; } return s; });
        sum   += run("H", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<shallow::H>(e.get()); s += p ? p->h : e->z; } return s; });
        sum   += run("Z", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<         Z>(e.get()); s += p ? p->z : e->z; } return s; });
        print_average(sum);
        printf("```\n\n");

        printf("Implementation: `kcl_dynamic_cast`\n");
        printf("```\n");
        sum = 0;
        dummy += run("A", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<         A*>(e.get()); s += p ? p->a : e->z; } return s; });
        sum   += run("B", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<shallow::B*>(e.get()); s += p ? p->b : e->z; } return s; });
        sum   += run("C", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<shallow::C*>(e.get()); s += p ? p->c : e->z; } return s; });
        sum   += run("D", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<shallow::D*>(e.get()); s += p ? p->d : e->z; } return s; });
        sum   += run("E", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<shallow::E*>(e.get()); s += p ? p->e : e->z; } return s; });
        sum   += run("F", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<shallow::F*>(e.get()); s += p ? p->f : e->z; } return s; });
        sum   += run("G", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<shallow::G*>(e.get()); s += p ? p->g : e->z; } return s; });
        sum   += run("H", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<shallow::H*>(e.get()); s += p ? p->h : e->z; } return s; });
        sum   += run("Z", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<         Z*>(e.get()); s += p ? p->z : e->z; } return s; });
        print_average(sum);
        printf("```\n\n");

    } else if (h == Hierarchy::balanced) {
        printf("Implementation: `dynamic_cast`\n");
        printf("```\n");
        sum = 0;
        dummy += run("A", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<          A*>(e.get()); s += p ? p->a : e->z; } return s; });
        sum   += run("B", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<balanced::B*>(e.get()); s += p ? p->b : e->z; } return s; });
        sum   += run("C", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<balanced::C*>(e.get()); s += p ? p->c : e->z; } return s; });
        sum   += run("D", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<balanced::D*>(e.get()); s += p ? p->d : e->z; } return s; });
        sum   += run("E", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<balanced::E*>(e.get()); s += p ? p->e : e->z; } return s; });
        sum   += run("F", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<balanced::F*>(e.get()); s += p ? p->f : e->z; } return s; });
        sum   += run("G", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<balanced::G*>(e.get()); s += p ? p->g : e->z; } return s; });
        sum   += run("H", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<balanced::H*>(e.get()); s += p ? p->h : e->z; } return s; });
        sum   += run("Z", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<          Z*>(e.get()); s += p ? p->z : e->z; } return s; });
        print_average(sum);
        printf("```\n\n");

        printf("Implementation: `vrc::dynamicCast`\n");
        printf("```\n");
        sum = 0;
        dummy += run("A", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<          A>(e.get()); s += p ? p->a : e->z; } return s; });
        sum   += run("B", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<balanced::B>(e.get()); s += p ? p->b : e->z; } return s; });
        sum   += run("C", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<balanced::C>(e.get()); s += p ? p->c : e->z; } return s; });
        sum   += run("D", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<balanced::D>(e.get()); s += p ? p->d : e->z; } return s; });
        sum   += run("E", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<balanced::E>(e.get()); s += p ? p->e : e->z; } return s; });
        sum   += run("F", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<balanced::F>(e.get()); s += p ? p->f : e->z; } return s; });
        sum   += run("G", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<balanced::G>(e.get()); s += p ? p->g : e->z; } return s; });
        sum   += run("H", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<balanced::H>(e.get()); s += p ? p->h : e->z; } return s; });
        sum   += run("Z", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<          Z>(e.get()); s += p ? p->z : e->z; } return s; });
        print_average(sum);
        printf("```\n\n");

        printf("Implementation: `kcl_dynamic_cast`\n");
        printf("```\n");
        sum = 0;
        dummy += run("A", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<          A*>(e.get()); s += p ? p->a : e->z; } return s; });
        sum   += run("B", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<balanced::B*>(e.get()); s += p ? p->b : e->z; } return s; });
        sum   += run("C", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<balanced::C*>(e.get()); s += p ? p->c : e->z; } return s; });
        sum   += run("D", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<balanced::D*>(e.get()); s += p ? p->d : e->z; } return s; });
        sum   += run("E", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<balanced::E*>(e.get()); s += p ? p->e : e->z; } return s; });
        sum   += run("F", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<balanced::F*>(e.get()); s += p ? p->f : e->z; } return s; });
        sum   += run("G", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<balanced::G*>(e.get()); s += p ? p->g : e->z; } return s; });
        sum   += run("H", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<balanced::H*>(e.get()); s += p ? p->h : e->z; } return s; });
        sum   += run("Z", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<          Z*>(e.get()); s += p ? p->z : e->z; } return s; });
        print_average(sum);
        printf("```\n\n");

    } else if (h == Hierarchy::cross) {
        printf("Implementation: `dynamic_cast`\n");
        printf("```\n");
        sum = 0;
        dummy += run("A", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<       A*>(e.get()); s += p ? p->a : e->z; } return s; });
        sum   += run("B", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<cross::B*>(e.get()); s += p ? p->b : e->z; } return s; });
        sum   += run("C", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<cross::C*>(e.get()); s += p ? p->c : e->z; } return s; });
        sum   += run("D", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<cross::D*>(e.get()); s += p ? p->d : e->z; } return s; });
        sum   += run("E", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<cross::E*>(e.get()); s += p ? p->e : e->z; } return s; });
        sum   += run("F", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<cross::F*>(e.get()); s += p ? p->f : e->z; } return s; });
        sum   += run("G", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<cross::G*>(e.get()); s += p ? p->g : e->z; } return s; });
        sum   += run("H", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<cross::H*>(e.get()); s += p ? p->h : e->z; } return s; });
        sum   += run("Z", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = dynamic_cast<       Z*>(e.get()); s += p ? p->z : e->z; } return s; });
        print_average(sum);
        printf("```\n\n");

        printf("Implementation: `vrc::dynamicCast`\n");
        printf("```\n");
        sum = 0;
        dummy += run("A", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<       A>(e.get()); s += p ? p->a : e->z; } return s; });
        sum   += run("B", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<cross::B>(e.get()); s += p ? p->b : e->z; } return s; });
        sum   += run("C", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<cross::C>(e.get()); s += p ? p->c : e->z; } return s; });
        sum   += run("D", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<cross::D>(e.get()); s += p ? p->d : e->z; } return s; });
        sum   += run("E", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<cross::E>(e.get()); s += p ? p->e : e->z; } return s; });
        sum   += run("F", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<cross::F>(e.get()); s += p ? p->f : e->z; } return s; });
        sum   += run("G", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<cross::G>(e.get()); s += p ? p->g : e->z; } return s; });
        sum   += run("H", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<cross::H>(e.get()); s += p ? p->h : e->z; } return s; });
        sum   += run("Z", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = vrc::dynamicCast<       Z>(e.get()); s += p ? p->z : e->z; } return s; });
        print_average(sum);
        printf("```\n\n");

        printf("Implementation: `kcl_dynamic_cast`\n");
        printf("```\n");
        sum = 0;
        dummy += run("A", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<       A*>(e.get()); s += p ? p->a : e->z; } return s; });
        sum   += run("B", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<cross::B*>(e.get()); s += p ? p->b : e->z; } return s; });
        sum   += run("C", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<cross::C*>(e.get()); s += p ? p->c : e->z; } return s; });
        sum   += run("D", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<cross::D*>(e.get()); s += p ? p->d : e->z; } return s; });
        sum   += run("E", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<cross::E*>(e.get()); s += p ? p->e : e->z; } return s; });
        sum   += run("F", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<cross::F*>(e.get()); s += p ? p->f : e->z; } return s; });
        sum   += run("G", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<cross::G*>(e.get()); s += p ? p->g : e->z; } return s; });
        sum   += run("H", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<cross::H*>(e.get()); s += p ? p->h : e->z; } return s; });
        sum   += run("Z", [&v]() { uint64_t s = 0; for (auto&& e: v) { auto *p = kcl_dynamic_cast<       Z*>(e.get()); s += p ? p->z : e->z; } return s; });
        print_average(sum);
        printf("```\n\n");

    }
}

int main()
{
    auto vec_deep_successful = generate_data(Hierarchy::deep, 6, 0);
    auto vec_deep_fails = generate_data(Hierarchy::deep, 1, 0);
    auto vec_deep_mixed = generate_data(Hierarchy::deep, 0, 7);

    auto vec_shallow_successful = generate_data(Hierarchy::shallow, 6, 0);
    auto vec_shallow_fails = generate_data(Hierarchy::shallow, 1, 0);
    auto vec_shallow_mixed = generate_data(Hierarchy::shallow, 0, 7);

    auto vec_balanced_mixed = generate_data(Hierarchy::balanced, 0, 7);

    auto vec_cross_all = generate_data(Hierarchy::cross, 7, 0);
    auto vec_cross_most = generate_data(Hierarchy::cross, 6, 0);
    auto vec_cross_mixed = generate_data(Hierarchy::cross, 5, 2);

    // Run the benchmark loop 3 times:
    // 1st: Warming up, discard.
    // 2nd: Objects are ordered in memory
    // 3rd: Objects are shuffled in memory
    for (unsigned i = 0; i < 3; i++) {
        max_num_ops = 0;

        printf("\n\n\n\n\n");

        switch(i) {
            case 0:
                printf("## Run 0 (discard)\n\n");
                break;
            case 1:
                printf("## Run 1 (objects aligned)\n\n");
                break;
            case 2:
                printf("## Run 2 (objects shuffled)\n\n");

                shuffle(vec_deep_successful);
                shuffle(vec_deep_fails);
                shuffle(vec_deep_mixed);
                shuffle(vec_shallow_successful);
                shuffle(vec_shallow_fails);
                shuffle(vec_shallow_mixed);
                shuffle(vec_balanced_mixed);

                shuffle(vec_cross_all);
                shuffle(vec_cross_most);
                shuffle(vec_cross_mixed);
                break;
        }

        printf("### Class hierarchy: cross\n\n");

        printf("```\n");
        printf("[A]-+   C -+   E -+\n");
        printf("    |      |      |\n");
        printf("    +-> F -+-> G -+-> H\n");
        printf("    |      |\n");
        printf(" B -+   D -+\n");
        printf("\nZ - unrelated\n");
        printf("```\n");

        printf("#### Cast type: All successful (cast from class H)\n\n");
        run_benchmarks(vec_cross_all, Hierarchy::cross);

        printf("#### Cast type: Mostly successful (cast from class G)\n\n");
        run_benchmarks(vec_cross_most, Hierarchy::cross);

        printf("#### Cast type: Mixed (cast from random classes F, G, or H)\n\n");
        run_benchmarks(vec_cross_mixed, Hierarchy::cross);

        printf("### Class hierarchy: deep\n\n");

        printf("```\n");
        printf("[A]---> B ---> C ---> D ---> E ---> F ---> G ---> H\n");
        printf("\nZ - unrelated\n");
        printf("```\n");

        printf("#### Cast type: Mostly successful (cast from class G)\n\n");
        run_benchmarks(vec_deep_successful, Hierarchy::deep);

        printf("#### Cast type: Mostly failed (cast from class B)\n\n");
        run_benchmarks(vec_deep_fails, Hierarchy::deep);

        printf("#### Cast type: Mixed (cast from random classes)\n\n");
        run_benchmarks(vec_deep_mixed, Hierarchy::deep);


        printf("\n\n\n\n\n");
        printf("### Class hierarchy: shallow\n\n");

        printf("```\n");
        printf("    +-> B\n");
        printf("    |\n");
        printf("    +-> C\n");
        printf("    |\n");
        printf("    +-> D\n");
        printf("    |\n");
        printf("[A]-+-> E\n");
        printf("    |\n");
        printf("    +-> F\n");
        printf("    |\n");
        printf("    +-> G\n");
        printf("    |\n");
        printf("    +-> H\n");
        printf("\nZ - unrelated\n");
        printf("```\n");

        printf("#### Cast type: Mostly successful (cast from class G)\n\n");
        run_benchmarks(vec_shallow_successful, Hierarchy::shallow);

        printf("#### Cast type: Mostly failed (cast from class B)\n\n");
        run_benchmarks(vec_shallow_fails, Hierarchy::shallow);

        printf("#### Cast type: Mixed (cast from random classes)\n\n");
        run_benchmarks(vec_shallow_mixed, Hierarchy::shallow);


        printf("\n\n\n\n\n");
        printf("### Class hierarchy: balanced\n\n");

        printf("```\n");
        printf("    +-> B -+-> C\n");
        printf("    |      |\n");
        printf("[A]-+      +-> D\n");
        printf("    |\n");
        printf("    |      +-> F\n");
        printf("    |      |\n");
        printf("    +-> E -+-> G\n");
        printf("           |\n");
        printf("           +-> H\n");
        printf("\nZ - unrelated\n");
        printf("```\n");

        printf("#### Cast type: Mixed (cast from random classes)\n\n");
        run_benchmarks(vec_balanced_mixed, Hierarchy::balanced);
    }

    printf("\n\n\n\n\n");
    printf("sizeof JustKclRtti: %llu\n\n", static_cast<unsigned long long>(sizeof(JustKclRtti)));
    printf("sizeof JustVrcRtti: %llu\n\n", static_cast<unsigned long long>(sizeof(JustVrcRtti)));
    printf("sizeof JustRtti: %llu\n\n", static_cast<unsigned long long>(sizeof(JustRtti)));
    printf("sizeof A: %llu\n\n", static_cast<unsigned long long>(sizeof(A)));
    printf("%f", dummy);
}
