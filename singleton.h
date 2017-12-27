#ifndef SINGLETON_H
#define SINGLETON_H
#include <QDebug>

template<typename C>
class Singleton
{
public:
    static C* instance();
    virtual ~Singleton();
    static bool IsRuning;

protected:
    Singleton(){}

private:
    Singleton(const Singleton&);
    static C* _instance;
    Singleton& operator = (const Singleton&);
};

template<typename C>
C* Singleton<C>::instance()
{
    if(!_instance)
    {
        _instance = new C();
        IsRuning = true;
    }
    return _instance;
}

template<typename C>
Singleton<C>::~Singleton()
{
    if(0 != _instance)
    {
        delete _instance;
        _instance = 0;
    }
}
template<typename C>C* Singleton<C>::_instance = 0;
template<typename C> bool Singleton<C>::IsRuning = false;

#endif // SINGLETON_H
