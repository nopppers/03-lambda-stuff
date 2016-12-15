///////////////////////////////////
// Nothing
///////////////////////////////////
struct Nothing 
{
};

///////////////////////////////////
// Bools
///////////////////////////////////
struct true_type
{
};

struct false_type
{
};

///////////////////////////////////
// Result Of
///////////////////////////////////
template <typename F>
struct ResultOf
{
    typedef Nothing type;
};

///////////////////////////////////
// Val
///////////////////////////////////
template <typename T>
struct Val
{
    Val(const T &v) : value_(v) {}
    Val(const Val &v) : value_(v.value_) {}
    
    T operator()() const
    {
        return value_;
    }

    template <typename U>
    T operator()(const U &) const
    {
        return value_;
    }

    template <typename U, typename V>
    T operator()(const U &, const V &) const
    {
        return value_;
    }

    template <typename U, typename V, typename Z>
    T operator()(const U &, const V &, const Z &) const
    {
        return value_;
    }

    T value_;
};

template <typename T>
Val<T> val(const T &v)
{
    return Val<T>(v);
}

template <typename T>
struct ResultOf<Val<T> >
{
    typedef T type;
};

template <typename T, typename U>
struct ResultOf<Val<T>(U)>
{
    typedef T type;
};

template <typename T, typename U, typename V>
struct ResultOf<Val<T>(U, V)>
{
    typedef T type;
};

template <typename T, typename U, typename V, typename Z>
struct ResultOf<Val<T>(U, V, Z)>
{
    typedef T type;
};

///////////////////////////////////
// Ref
///////////////////////////////////
template <typename T>
struct Ref
{
    Ref(T &r) : ref_(r) {}
    Ref(const Ref &r) : ref_(r.ref_) {}

    T& operator()() const
    {
        return ref_;
    }

    template <typename U>
    T& operator()(const U &) const
    {
        return ref_;
    }

    template <typename U, typename V>
    T& operator()(const U &, const V &) const
    {
        return ref_;
    }

    template <typename U, typename V, typename Z>
    T& operator()(const U &, const V &, const Z &) const
    {
        return ref_;
    }

    T& ref_;
};

template <typename T>
Ref<T> ref(T &r)
{
    return Ref<T>(r);
}

template <typename T>
struct ResultOf<Ref<T> >
{
    typedef T& type;
};

template <typename T, typename U>
struct ResultOf<Ref<T>(U)>
{
    typedef T& type;
};

template <typename T, typename U, typename V>
struct ResultOf<Ref<T>(U, V)>
{
    typedef T& type;
};

template <typename T, typename U, typename V, typename Z>
struct ResultOf<Ref<T>(U, V, Z)>
{
    typedef T& type;
};

///////////////////////////////////
// Arg
///////////////////////////////////
template <size_t I>
struct Arg
{
};

template <>
struct Arg<1>
{
    Arg(){}

    template <typename T, typename U, typename V>
    T operator()(const T &t, const U &, const V &) const
    {
        return t;
    }

    template <typename T, typename U>
    T operator()(const T &t, const U &) const
    {
        return t;
    }

    template <typename T>
    T operator()(const T &t) const
    {
        return t;
    }
};

template <>
struct Arg<2>
{
    Arg(){}

    template <typename T, typename U, typename V>
    U operator()(const T &, const U &u, const V &) const
    {
        return u;
    }

    template <typename T, typename U>
    U operator()(const T &, const U &u) const
    {
        return u;
    }
};

template <>
struct Arg<3>
{
    Arg(){}

    template <typename T, typename U, typename V>
    V operator()(const T &, const U &, const V &v) const
    {
        return v;
    }
};

const Arg<1> arg1;
const Arg<2> arg2;
const Arg<3> arg3;

template <size_t I, typename T, typename U = Nothing, typename V = Nothing>
struct ArgReturnType
{};

template <typename T, typename U, typename V>
struct ArgReturnType<1, T, U, V>
{
    typedef T type;
};

template <typename T, typename U, typename V>
struct ArgReturnType<2, T, U, V>
{
    typedef U type;
};

template <typename T, typename U, typename V>
struct ArgReturnType<3, T, U, V>
{
    typedef V type;
};

template <typename T, typename U>
struct ArgReturnType<1, T, U>
{
    typedef T type;
};

template <typename T, typename U>
struct ArgReturnType<2, T, U>
{
    typedef U type;
};

template <typename T>
struct ArgReturnType<1, T>
{
    typedef T type;
};

template <size_t argN, typename T, typename U, typename V>
struct ResultOf<Arg<argN>(T, U, V)>
{
    typedef typename ArgReturnType<argN, T, U, V>::type type;
};

template <size_t argN, typename T, typename U>
struct ResultOf<Arg<argN>(T, U)>
{
    typedef typename ArgReturnType<argN, T, U>::type type;
};

template <size_t argN, typename T>
struct ResultOf<Arg<argN>(T)>
{
    typedef typename ArgReturnType<argN, T>::type type;
};

///////////////////////////////////
// Function
///////////////////////////////////
template <typename Func, typename Arg1 = Nothing, typename Arg2 = Nothing, typename Arg3 = Nothing>
struct LazyFunction;

template <typename Func>
struct LazyFunction<Func, Nothing, Nothing, Nothing>
{
    Func functor_;
    LazyFunction(Func functor) : functor_(functor) {}
};

template <typename Func, typename Thing1>
struct LazyFunction<Func, Thing1, Nothing, Nothing>
{
    Func functor_;
    Thing1 thing1_;

    typedef Func functor_type;

    LazyFunction(Func functor, Thing1 thing1) : functor_(functor), thing1_(thing1) {}

    template <typename T, typename U, typename V>
    typename ResultOf<
        Func(typename ResultOf<Thing1(T, U, V)>::type)
    >::type operator()(const T &t, const U &u, const V &v) const
    {
        return functor_(thing1_(t, u, v));
    }

    template <typename T, typename U>
    typename ResultOf<
        Func(typename ResultOf<Thing1(T, U)>::type)
    >::type operator()(const T &t, const U &u) const
    {
        return functor_(thing1_(t, u));
    }

    template <typename T>
    typename ResultOf<
        Func(typename ResultOf<Thing1(T)>::type)
    >::type operator()(const T &t) const
    {
        return functor_(thing1_(t));
    }

};

template <typename Func, typename Thing1, typename Thing2>
struct LazyFunction<Func, Thing1, Thing2, Nothing>
{
    Func functor_;
    Thing1 thing1_;
    Thing2 thing2_;

    typedef Func functor_type;

    LazyFunction(Func functor, Thing1 thing1, Thing2 thing2) 
        : functor_(functor), thing1_(thing1), thing2_(thing2) {}

    template <typename T, typename U, typename V>
    typename ResultOf<
        Func(typename ResultOf<Thing1(T, U, V)>::type,
             typename ResultOf<Thing2(T, U, V)>::type)
    >::type operator()(const T &t, const U &u, const V &v) const
    {
        return functor_(thing1_(t, u, v), thing2_(t, u, v));
    }

    template <typename T, typename U>
    typename ResultOf<
        Func(typename ResultOf<Thing1(T, U)>::type,
             typename ResultOf<Thing2(T, U)>::type)
    >::type operator()(const T &t, const U &u) const
    {
        return functor_(thing1_(t, u), thing2_(t, u));
    }

    template <typename T>
    typename ResultOf<
        Func(typename ResultOf<Thing1(T)>::type,
             typename ResultOf<Thing2(T)>::type)
    >::type operator()(const T &t) const
    {
        return functor_(thing1_(t), thing2_(t));
    }

};

template <typename Func, typename Thing1, typename Thing2, typename Thing3>
struct LazyFunction
{
    Func functor_;
    Thing1 thing1_;
    Thing2 thing2_;
    Thing3 thing3_;

    typedef Func functor_type;

    LazyFunction(Func functor, Thing1 thing1, Thing2 thing2, Thing3 thing3)
        : functor_(functor), thing1_(thing1), thing2_(thing2), thing3_(thing3) {}

    template <typename T, typename U, typename V>
    typename ResultOf<
        Func(typename ResultOf<Thing1(T, U, V)>::type,
             typename ResultOf<Thing2(T, U, V)>::type,
             typename ResultOf<Thing3(T, U, V)>::type)
    >::type operator()(const T &t, const U &u, const V &v) const
    {
        return functor_(thing1_(t, u, v), thing2_(t, u, v), thing3_(t, u, v));
    }

    template <typename T, typename U>
    typename ResultOf<
        Func(typename ResultOf<Thing1(T, U)>::type,
             typename ResultOf<Thing2(T, U)>::type,
             typename ResultOf<Thing3(T, U)>::type)
    >::type operator()(const T &t, const U &u) const
    {
        return functor_(thing1_(t, u), thing2_(t, u), thing3_(t, u));
    }

    template <typename T>
    typename ResultOf<
        Func(typename ResultOf<Thing1(T)>::type,
             typename ResultOf<Thing2(T)>::type,
             typename ResultOf<Thing3(T)>::type)
    >::type operator()(const T &t) const
    {
        return functor_(thing1_(t), thing2_(t), thing3_(t));
    }

};


template <typename T>
struct Converter
{
    typedef Val<T> type;
};

template <typename T>
struct Converter<Val<T> >
{
    typedef Val<T> type;
};

template <typename T>
struct Converter<Ref<T> >
{
    typedef Ref<T> type;
};

template <size_t I>
struct Converter<Arg<I> >
{
    typedef Arg<I> type;
};

template <typename Func, typename T, typename U, typename V>
struct Converter<LazyFunction<Func, T, U, V> >
{
    typedef LazyFunction<Func, T, U, V> type;
};

template <typename Func>
struct Function
{
    Func functor_;

    Function() {}

    LazyFunction<Func> operator()() const
    {
        return LazyFunction<Func>(functor_);
    }

    template <typename T>
    LazyFunction<Func, typename Converter<T>::type> operator()(const T &t) const
    {
        typedef typename Converter<T>::type converter;
        return LazyFunction<Func, converter>(functor_, converter(t));
    }

    template <typename T, typename U>
    LazyFunction<Func, typename Converter<T>::type, typename Converter<U>::type>
        operator()(const T &t, const U &u) const
    {
        typedef typename Converter<T>::type converter1;
        typedef typename Converter<U>::type converter2;
        return LazyFunction<Func, converter1, converter2>(
            functor_, converter1(t), converter2(u));
    }

    template <typename T, typename U, typename V>
    LazyFunction<Func, typename Converter<T>::type, typename Converter<U>::type,
        typename Converter<V>::type> operator()(const T &t, const U &u, const V &v) const
    {
        typedef typename Converter<T>::type converter1;
        typedef typename Converter<U>::type converter2;
        typedef typename Converter<V>::type converter3;
        return LazyFunction<Func, converter1, converter2, converter3>(
            functor_, converter1(t), converter2(u), converter3(v));
    }
};

template <typename Func, typename T>
struct ResultOf<Function<Func>(T)>
{
    typedef LazyFunction<Func, typename Converter<T>::type> type; 
};

template <typename Func, typename T, typename U>
struct ResultOf<Function<Func>(T, U)>
{
    typedef LazyFunction<Func, 
        typename Converter<T>::type,
        typename Converter<U>::type> type;
};

template <typename Func, typename T, typename U, typename V>
struct ResultOf<Function<Func>(T, U, V)>
{
    typedef LazyFunction<Func,
        typename Converter<T>::type,
        typename Converter<U>::type,
        typename Converter<V>::type> type;
};

template <typename Func, typename Thing1, typename T>
struct ResultOf<LazyFunction<Func, Thing1>(T)>
{
    typedef typename ResultOf<
        Func(typename ResultOf<Thing1(T)>::type)>
            ::type type;
};

template <typename Func, typename Thing1, typename T, typename U>
struct ResultOf<LazyFunction<Func, Thing1>(T, U)>
{
    typedef typename ResultOf<
        Func(typename ResultOf<Thing1(T, U)>::type)
            >::type type;
};

template <typename Func, typename Thing1, typename T, typename U, typename V>
struct ResultOf<LazyFunction<Func, Thing1>(T, U, V)>
{
    typedef typename ResultOf<
        Func(typename ResultOf<Thing1(T, U, V)>::type)
            >::type type;
};

template <typename Func, typename Thing1, typename Thing2, typename T>
struct ResultOf<LazyFunction<Func, Thing1, Thing2>(T)>
{
    typedef typename ResultOf<
        Func(typename ResultOf<Thing1(T)>::type,
             typename ResultOf<Thing2(T)>::type)
            >::type type;
};

template <typename Func, typename Thing1, typename Thing2, typename T, typename U>
struct ResultOf<LazyFunction<Func, Thing1, Thing2>(T, U)>
{
    typedef typename ResultOf<
        Func(typename ResultOf<Thing1(T, U)>::type,
             typename ResultOf<Thing2(T, U)>::type)
            >::type type;
};

template <typename Func, typename Thing1, typename Thing2, typename T, typename U, typename V>
struct ResultOf<LazyFunction<Func, Thing1, Thing2>(T, U, V)>
{
    typedef typename ResultOf<
        Func(typename ResultOf<Thing1(T, U, V)>::type,
             typename ResultOf<Thing2(T, U, V)>::type)
            >::type type;
};

template <typename Func, typename Thing1, typename Thing2, typename Thing3, typename T>
struct ResultOf<LazyFunction<Func, Thing1, Thing2, Thing3>(T)>
{
    typedef typename ResultOf<
        Func(typename ResultOf<Thing1(T)>::type,
             typename ResultOf<Thing2(T)>::type,
             typename ResultOf<Thing3(T)>::type)
            >::type type;
};

template <typename Func, typename Thing1, typename Thing2, typename Thing3, typename T, typename U>
struct ResultOf<LazyFunction<Func, Thing1, Thing2, Thing3>(T, U)>
{
    typedef typename ResultOf<
        Func(typename ResultOf<Thing1(T, U)>::type,
             typename ResultOf<Thing2(T, U)>::type,
             typename ResultOf<Thing3(T, U)>::type)
            >::type type;
};

template <typename Func, typename Thing1, typename Thing2, typename Thing3, typename T, typename U, typename V>
struct ResultOf<LazyFunction<Func, Thing1, Thing2, Thing3>(T, U, V)>
{
    typedef typename ResultOf<
        Func(typename ResultOf<Thing1(T, U, V)>::type,
             typename ResultOf<Thing2(T, U, V)>::type,
             typename ResultOf<Thing3(T, U, V)>::type)
            >::type type;
};

///////////////////////////////////
// Compatible Functors
///////////////////////////////////
struct Identity_impl
{
    template <typename T>
    T operator()(const T &t) const
    {
        return t;
    }
};

struct Add_impl
{
    template <typename T, typename U>
    T operator()(const T &a, const U &b) const
    {
        return a + b;
    }
};

struct AddTo_impl
{
    template <typename T, typename U>
    T& operator()(T &a, const U &b) const
    {
        return a += b;
    }
};

struct Sub_impl
{
    template <typename T, typename U>
    T operator()(const T &a, const U &b) const
    {
        return a - b;
    }
};

struct Mul_impl
{
    template <typename T, typename U>
    T operator()(const T &a, const U &b) const
    {
        return a * b;
    }
};

struct MAdd_impl
{
    template <typename T, typename U, typename V>
    T operator()(const T &a, const U &b, const V &c) const
    {
        return a*b + c;
    }
};

struct Eq_impl
{
    template <typename T, typename U>
    bool operator()(const T &a, const U &b) const
    {
        return a == b;
    }
};

struct Length_impl
{
    template <typename T>
    size_t operator()(const T &a) const
    {
        return a.length();
    }
};

struct Substr_impl
{
    template <typename T>
    T operator()(const T &a, size_t pos = 0, size_t count = T::npos) const
    {
        return a.substr(pos, count);
    }
};

template <typename T>
struct ResultOf<Identity_impl(T)>
{
    typedef T type;
};

template <typename T, typename U>
struct ResultOf<Add_impl(T, U)>
{
    typedef T type;
};

template <typename T, typename U>
struct ResultOf<AddTo_impl(T&, U)>
{
    typedef T& type;
};

template <typename T, typename U>
struct ResultOf<Sub_impl(T, U)>
{
    typedef T type;
};

template <typename T, typename U, typename V>
struct ResultOf<MAdd_impl(T, U, V)>
{
    typedef T type;
};

template <typename T, typename U>
struct ResultOf<Eq_impl(T, U)>
{
    typedef bool type;
};

template <typename T>
struct ResultOf<Length_impl(T)>
{
    typedef size_t type;
};

template <typename T, typename U, typename V>
struct ResultOf<Substr_impl(T, U, V)>
{
    typedef T type;
};

const Function<Identity_impl> Identity;
const Function<Add_impl> Add;
const Function<AddTo_impl> AddTo;
const Function<Sub_impl> Sub;
const Function<Mul_impl> Mul;
const Function<MAdd_impl> MAdd;
const Function<Eq_impl> Eq;
const Function<Length_impl> Length;
const Function<Substr_impl> Substr;
