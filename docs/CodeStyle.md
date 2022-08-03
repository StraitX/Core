# StraitX Engine codestyle

- files and folders names are snake_case (all lowercase with underscores)
```
core/database_storage.cpp
```
- typenames names are MixedCase
```c++
class MyExitingClass{...};
struct Data{...};
typedef int Flags;
```
- functions are MixedCase
```c++
PerformTask();
```
- local variables are snake_case
```c++
int players_count;
GameObject main_boss;
```
- private member variables have prefix m_ and are MixedCase
```c++
class Entity{
    AnotherClass m_Variable
    ...
};
```
- public members are all MixedCase
```c++
struct Props{
    unsigned Width;
    unsigned Height;
};
```
- static variables are named with prefix s_ and are MixedCase. In case of being member variable, they don't have m prefix

```c++
class GameObject{
    static String s_ClassName = "GameObject"
    ...
};
```
- getters are named same name as member variable except its prefix
- setters are named like a regular function
```c++
class MyClass{
    int m_Counter;

    int Counter();
    void SetCounter(int counter);
};
```
- constant variables are MixedCase. If const variable is static, s_ prefix should be ignored
```c++
const String ApplicationName = "The Best Game"
```
- enums members all are MixedCase (like constants)
```c++
enum KeyCodes{
    LeftAlt,
    RightAlt,
    ...  
};
```

- implementation functions that should exist for some reason in global scope should have underscore in the beginning 

```c++
extern void _AssertImpl(...);

#define _SX_MACRO_IMPL(...)
```
