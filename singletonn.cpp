#include "singletonn.h"


//template<typename C>
//void Singleton<C>::instance()
//{
////    if(!_instance)
////        _instance = new C();
////    return _instance;
//}
//template<typename C>
//Singleton<C>::~Singleton()
//{
//    if(0 != Singleton::_instance)
//    {
//        delete Singleton::_instance;
//        Singleton::_instance = 0;
//    }
//}
//template<typename C>C* Singleton<C>::_instance = 0;
template<typename C> int Singleton<C>::s = 8;
