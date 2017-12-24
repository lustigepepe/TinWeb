#ifndef Singleton_H
#define Singleton_H

template<class C>
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

#endif // Singleton_H
