#pragma once
//生成对应__Thief__##name
#define PRODUCE_THIEF(name) \
template<auto M> struct __Thief__##name;\
template<typename T, typename U, U T::* M>\
struct __Thief__##name<M> {\
	friend inline U& ##name(T& u) {\
		return u.*M;\
	}\
};
//生成偷窃模板函数 ##name
//name为你要定义的函数名，同一cpp内不支持重载
//_class为窃取目标所属class类型
//_Ty为目标数据类型，_Val为目标名称
#define THIEF_TEMPLATE(name,_class,_Ty,_Val) \
PRODUCE_THIEF(name);\
template struct __Thief__##name<&_class::_Val>; _Ty& ##name(_class&);