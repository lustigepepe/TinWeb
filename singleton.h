#ifndef SINGLETON_H
#define SINGLETON_H



template<typename C>
class Singleton
{
public:
    static C* instance();
private:
    static C* _instance;
    Singleton(){}
    Singleton(const Singleton& );
    Singleton& operator = (const Singleton&);
    ~Singleton();
};
template<typename C>
C* Singleton<C>::instance()
{
    if(!_instance)
        _instance = new C();
    return _instance;
}
template<typename C>
Singleton<C>::~Singleton()
{
    if(0 != Singleton::_instance)
    {
        delete Singleton::_instance;
        Singleton::_instance = 0;
    }
}
template<typename C>C* Singleton<C>::_instance = 0;


#endif // SINGLETON_H
