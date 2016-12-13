
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
    T operator()(const U &u)
    {
        return value_;
    }

    template <typename U, typename V>
    T operator()(const U &u, const V &v)
    {
        return value_;
    }

    template <typename U, typename V, typename Z>
    T operator()(const U &u, const V &v, const Z &z)
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

///////////////////////////////////
// Ref
///////////////////////////////////
template <typename T>
struct Ref
{
    Ref(T &r) : ref_(r) {}
    T& operator()()
    {
        return ref_;
    }

    template <typename U>
    T& operator()(const U &u)
    {
        return ref_;
    }

    template <typename U, typename V>
    T& operator()(const U &u, const V &v)
    {
        return ref_;
    }

    template <typename U, typename V, typename Z>
    T& operator()(const U &u, const V &v, const Z &z)
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


///////////////////////////////////
// Function
///////////////////////////////////

struct Nothing 
{
};

template <typename F>
struct ResultOf {};

template <typename Func, typename Arg1 = Nothing, typename Arg2 = Nothing, typename Arg3 = Nothing>
struct LazyFunction
{
};

template <typename Func>
struct LazyFunction<Func, Nothing, Nothing, Nothing>
{
    Func functor_;

    LazyFunction(Func functor) : functor_(functor) {}

    template <typename T, typename U, typename V>
    typename ResultOf<Func(T, U, V)>::type operator()(const T &t, const U &u, const V &v) const
    {
        return functor_(t, u, v);
    }

    template <typename T, typename U>
    typename ResultOf<Func(T, U)>::type operator()(const T &t, const U &u) const
    {
        return functor_(t, u);
    }

    template <typename T>
    typename ResultOf<Func(T)>::type operator()(const T &t) const
    {
        return functor_(t);
    }

    template <typename T = Func>
    // hmm... that T shouldn't be there...
    typename ResultOf<Func(T)>::type operator()() const
    {
        return functor_();
    }
};

template <typename Func, typename Thing1>
struct LazyFunction<Func, Thing1, Nothing, Nothing>
{
    Func functor_;
    Thing1 thing1_;

    LazyFunction(Func functor, Thing1 thing1) : functor_(functor), thing1_(thing1) {}

    template <typename T, typename U, typename V>
    typename ResultOf<
        Func(typename ResultOf<Thing1(T, U, V)>::type, U, V)
    >::type operator()(const T &t, const U &u, const V &v) const
    {
        return functor_(thing1_(t, u, v), u, v);
    }

    template <typename T, typename U>
    typename ResultOf<
        Func(T, U)>::type operator()(const T &t, const U &u) const
    {
        return functor_(thing1_(t, u), u);
    }

    template <typename T>
    typename ResultOf<Func(T)>::type operator()(const T &t) const
    {
        return functor_(thing1_(t));
    }

    template <typename T = Func>
    // hmm... that T shouldn't be there...
    typename ResultOf<Func(T)>::type operator()() const
    {
        return functor_();
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

    template <typename T = Func>
    LazyFunction<Func> operator()() const
    {
        // Return a functor that can be called directly with the full args
        return LazyFunction<Func>(functor_);
    }

    template <typename T>
    LazyFunction<Func> operator()(const T &t) const
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