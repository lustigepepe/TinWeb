#include "singleton.h"

template<class C>C* Singleton<C>::_instance = 0;

template<class C> C* Singleton<C>::instance()
{
    if(!_instance)
        _instance = new C();
    return _instance;
}

template<class C> Singleton<C>::~Singleton()
{
    if(0 !=Singleton::_instance)
    {
        delete Singleton::_instance;
        Singleton::_instance = 0;
    }
}
