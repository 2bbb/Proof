//
//  Utilities.h
//  Proof
//
//  Created by ISHII 2bit on 2015/01/04.
//  Copyright (c) 2015年 bufferRenaiss. All rights reserved.
//

#pragma once

#include "Core/Constants.h"
#include "Core/Macros.h"

template <typename, typename>
struct Assert;

template <typename A>
struct Assert<A, A> {};

template <typename>
struct Enable;

template <>
struct Enable<True> {};

//namespace {
//    template <typename T>
//    struct reduction_impl {
//        using type = T;
//    };
//    
//    template <typename T>
//    struct reduction_impl_template {
//        template <typename ... Ts>
//        using type = typename T::template type<Ts ...>;
//    };
//    
////    template <typename T, typename ... Ts, template <typename ...> class F = reduction_impl_template<T, Ts ...>>
////    struct reduction_impl {
////        using type = typename T::type;
////    };
//};

template <typename T>
using Reduction = typename T::type;

template <typename T>
using Eval = typename T::value;

template <typename ...>
struct TypeHolder {};

namespace {
    template <template <typename ...> class Template, typename A>
    struct rewrap_typeholder_impl;

    template <template <typename ...> class Template, typename ... As>
    struct rewrap_typeholder_impl<Template, TypeHolder<As ...>> {
        Type(NewTemplateType) type = Template<As ...>;
    };
};

template <template <typename ...> class Template, typename A>
Function() RewrapTypeHolder = Reduction<rewrap_typeholder_impl<Template, A>>;

struct Null {};

template <typename A, typename B>
struct Pair {};

namespace {
    struct inner_null {};
    template <typename A, typename B>
    struct inner_pair {};
};

template <typename ...>
struct Tuple {};

namespace {
    template <typename ...>
    struct flatten_impl;

    template <typename A>
    struct flatten_impl<A> {
        using type = TypeHolder<A>;
    };

    template <typename A, typename B>
    struct flatten_impl<inner_pair<A, B>> {
        using type = Reduction<flatten_impl<B, TypeHolder<A>>>;
    };

    template <typename A, typename ... Bs>
    struct flatten_impl<A, TypeHolder<Bs ...>> {
        using type = TypeHolder<Bs ..., A>;
    };

    template <typename A, typename B, typename ... Bs>
    struct flatten_impl<inner_pair<A, B>, TypeHolder<Bs ...>> {
        using type = Reduction<flatten_impl<B, TypeHolder<Bs ..., A>>>;
    };

    template <>
    struct flatten_impl<inner_null> {
        using type = TypeHolder<>;
    };
};

template <typename ... As>
using Flatten = Reduction<flatten_impl<As ...>>;

namespace {
    template <typename ...>
    struct make_unique_impl;

    template <typename A>
    struct make_unique_impl<A> {
        using type = A;
    };

    template <typename A>
    struct make_unique_impl<A, TypeHolder<>, TypeHolder<>> {
        using type = A;
    };

    template <typename A, typename ... Bs>
    struct make_unique_impl<A, Bs ...> {
        using type = Reduction<make_unique_impl<A, TypeHolder<Bs ...>, TypeHolder<>>>;
    };

    template <typename A, typename ... Bs, typename ... Cs>
    struct make_unique_impl<A, TypeHolder<A, Bs ...>, TypeHolder<Cs ...>> {
        using type = Reduction<make_unique_impl<A, TypeHolder<Bs ...>, TypeHolder<Cs ...>>>;
    };

    template <typename A, typename B, typename ... Cs, typename ... Ds>
    struct make_unique_impl<A, TypeHolder<B, Cs ...>, TypeHolder<Ds ...>> {
        using type = Reduction<make_unique_impl<A, TypeHolder<Cs ...>, TypeHolder<Ds ..., B>>>;
    };

    template <typename A, typename ... Bs>
    struct make_unique_impl<A, TypeHolder<>, TypeHolder<Bs ...>> {
        using type = inner_pair<A, Reduction<make_unique_impl<Bs ...>>>;
    };

    template <>
    struct make_unique_impl<> {
        using type = inner_null;
    };
};

template <typename ... As>
using MakeUnique = Flatten<Reduction<make_unique_impl<As ...>>>;

//template <template <typename> class F, template <typename> class G>
//struct compositie_impl {
//    template <typename T>
//    using type = F<G<T>>;
//};

template <typename X, template <typename ...> class F>
struct bind_impl {
    template <typename ... As>
    using type = F<X, As ...>;
};