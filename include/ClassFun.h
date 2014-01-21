#pragma once

#include "BaseFun.h"
#include "LuaName.h"
#include <string>

namespace sel {

template <int N, typename Ret, typename... Args>
class ClassFun : public BaseFun {
private:
    using _return_type = Ret;
    using _fun_type = std::function<Ret(Args...)>;
    _fun_type _fun;

public:
    ClassFun(lua_State *l,
             const std::string &name,
             _return_type(*fun)(Args...))
        : ClassFun(l, name, _fun_type{fun}) {}

    ClassFun(lua_State *l,
             const std::string &name,
             _fun_type fun) : _fun(fun) {
        lua_pushlightuserdata(l, (void *)static_cast<BaseFun *>(this));
        lua_pushcclosure(l, &detail::_lua_dispatcher, 1);
        lua_setfield(l, -2, name.c_str());
    }

    // Each application of a function receives a new Lua context so
    // this argument is necessary.
    int Apply(lua_State *l) {
        std::tuple<Args...> args = detail::_get_args<Args...>(l);
        _return_type value = detail::_lift(_fun, args);
        detail::_push(l, std::forward<_return_type>(value));
        return N;
    }
};

template <typename Ret, typename... Args>
class ClassFun<0, Ret, Args...> : public BaseFun {
private:
    using _return_type = Ret;
    using _fun_type = std::function<Ret(Args...)>;
    _fun_type _fun;

public:
    ClassFun(lua_State *l,
             const std::string &name,
             _return_type(*fun)(Args...))
        : ClassFun(l, name, _fun_type{fun}) {}

    ClassFun(lua_State *l,
             const std::string &name,
             _fun_type fun) : _fun(fun) {
        lua_pushlightuserdata(l, (void *)static_cast<BaseFun *>(this));
        lua_pushcclosure(l, &detail::_lua_dispatcher, 1);
        lua_setfield(l, -2, name.c_str());
    }

    // Each application of a function receives a new Lua context so
    // this argument is necessary.
    int Apply(lua_State *l) {
        std::tuple<Args...> args = detail::_get_args<Args...>(l);
        detail::_lift(_fun, args);
        return 0;
    }
};
}