#ifndef MOZI_CORE_COMMON_SINGLETON_H
#define MOZI_CORE_COMMON_SINGLETON_H

template <typename T>
struct HasShutdown {
    template <typename Class>
    static constexpr bool Test(decltype(&Class::Shutdown)*) 
    {
        return true;
    }

    template <typename>
    static constexpr bool Test(...)
    {
        return false;
    }
    static constexpr bool value = Test<T>(nullptr);
};

template <typename T>
constexpr bool HasShutdown<T>::value;

template <typename T>
typename std::enable_if<HasShutdown<T>::value>::type CallShutdown(T* instance)
{
    instance->Shutdown();
}

template <typename T>
typename std::enable_if<!HasShutdown<T>::value>::type CallShutdown(T* instance)
{
    (void*)instance;
}

#define DECLARE_SINGLETON(classname) \
    public:                         \
    static classname* Instance(bool create_if_needed = true) {          \
        static classname* Instance = nullptr;                           \
        if (!instance && create_if_needed) {                            \
            static std::once_flag flag;                                 \
            std::call_once(flag, [&]{instance = new (std::nothrow) classname(); }); \
        }                                                               \
        return instance;                                                \
    }                                                                   \
                                                                        \
    static void CleanUp() {                                             \
        auto instance = instance(false);                                \
        if (instance != nullptr) {                                      \
            CallShutdown(instance);                                     \
        }                                                               \
    }                                                                   \
    private:                                                            \
    classname();                                                        \
    classname(const classname&) = delete;                               \
    classname& operator=(const classname&) = delete;                    \

#endif
