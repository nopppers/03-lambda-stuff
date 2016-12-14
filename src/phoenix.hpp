///////////////////////////////////
// Nothing
///////////////////////////////////
struct Nothing 
{
};

///////////////////////////////////
// Result Of
///////////////////////////////////
template <typename F>
struct ResultOf {};

///////////////////////////////////
// Val
///////////////////////////////////
template <typename T>
struct Val
{
    Val(const T &v) : value_(v) {}
    
    T operator()() const
    {
        return value_;
    }

    template <typename U>
    T operator()(const U &u) const
    {
        return value_;
    }

    template <typename U, typename V>
    T operator()(const U &u, const V &v) const
    {
        return value_;
    }

    template <typename U, typename V, typename Z>
    T operator()(const U &u, const V &v, const Z &z) const
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
struct ResultOf<Val<T>()>
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
    T& operator()() const
    {
        return ref_;
    }

    template <typename U>
    T& operator()(const U &u) const
    {
        return ref_;
    }

    template <typename U, typename V>
    T& operator()(const U &u, const V &v) const
    {
        return ref_;
    }

    template <typename U, typename V, typename Z>
    T& operator()(const U &u, const V &v, const Z &z) const
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
struct ResultOf<Ref<T>()>
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
    template <typename T, typename U, typename V>
    T operator()(const T &t, const U &u, const V &v) const
    {
        return t;
    }

    template <typename T, typename U>
    T operator()(const T &t, const U &u) const
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
    template <typename T, typename U, typename V>
    U operator()(const T &t, const U &u, const V &v) const
    {
        return u;
    }

    template <typename T, typename U>
    U operator()(const T &t, const U &u) const
    {
        return u;
    }
};

template <>
struct Arg<3>
{
    template <typename T, typename U, typename V>
    V operator()(const T &t, const U &u, const V &v) const
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
struct LazyFunction
{
};

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

template <typename T>
struct Converter
{

};

template <typename Func>
struct Function
{
    Func functor_;

    LazyFunction<Func> operator()() const
    {
        return LazyFunction<Func>(functor_);
    }

    template <typename T>
    LazyFunction<Func, T> operator()(const T &t) const
    {
        return LazyFunction<Func, T>(functor_, t);
    }
};

struct Identity_impl
{
    template <typename T>
    T operator()(const T &t) const
    {
        return t;
    }
};

template <typename T>
struct ResultOf<Identity_impl(T)>
{
    typedef T type;
};

const Function<Identity_impl> Identity;
