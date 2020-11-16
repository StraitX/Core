# StraitX Engine codestyle

- files and folders names are all lowercase with underscores
```
core/database_storage.cpp
```
- typenames names are Mixed case
```c++
class MyExitingClass{...};
struct Data{...};
typedef int Flags;
```
- functions are Mixed case
```c++
PerformTask();
```
- regular variables are cameCase
```c++
int playersCount;
GameObject mainBoss;
```
- member variables have prefix m_
```c++
class Entity{
    AnotherClass m_Variable
    ...
};
```
- struct members are all mixed case
```c++
struct Props{
    unsigned Width;
    unsigned Height;
};
```
- static variables are named with prefix s. In case of being member variable, they don't have m prefix

```c++
class GameObject{
    String s_Name = "GameObject"
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
- constant variables have prefix k. If const variable is static, s_ prefix should be ignored
```c++
String kApplicationName = "TheBestApp"
```
- enums members all are mixed case
```c++
enum KeyCodes{
    LeftAlt,
    RightAlt,
    ...  
};
```