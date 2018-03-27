# Dimensions and Units
## Why use strong typing
One of the great things about C++ is that is a strongly typed language. This can help eliminate all sorts
of runtime bugs.

Typing means that you can label something, a variable say, as meaning something more than just a number.

for example you could decalare a function like this:

```c++
// poor use of typing
float double_frequency(float frequency);
```
The problem with this is the compiler really has no idea if the user is really passing a frequency
or they have made a mistake and are instead passing something else. This can lead to many fun hours
with the debugger.

```c++
float t = 0.7678;
float f = 1800;

float doubled_freq = double_frequency(t); // bug not caught by the compiler
```

If instead we use define some type to represent frequency, then 
```c++
Frequency<float> double_frequency(Frequency<float> frequency);
```
Now our user will have the help of the compiler, as their code will not compile with the bug.

```c++
float t = 0.7678;
Frequency<float> f(1800 * hertz);

Frequency<float> doubled_freq = double_frequency(t); // bug caught by the compiler
```

So strong typing can save you and your users hours of debugging frustrating runtime errors.

## Boost Quantites
The boost::units library we use is a very powerful tool that takes typing to the next level. It will perform a full
dimensional analysis at compile time (i.e. no runtime cost) ensuring your variables are consistent with physics.
e.g.
```C++
// work funtion will compile OK
boost::units::quantity<energy> work(boost::units::quantity<force> f, boost::units::quantity<length> d) {
    return f * d;
} 

// this work function will fail to compile as its dimensionally inconsistent
boost::units::quantity<length> work(boost::units::quantity<force> f, boost::units::quantity<length> d) {
    return f * d;
} 
```
The boost::units::quantity is a templated class that takes two template parameters. The first one is the unit (not just a dimension,
but also the unit in that dimension e.g. seconds, milliseconds, etc).
The second paramter is the actual numerical type that will be stored (float, int, unsigned, etc), with the default being a double.

Our own units are based on top of the boost::units SI system of units. They are desinged to be fully comaptible with the boost::units::si system
but also allow us to use std::chrono types to represent time and still benefit from all boost::units has to offer. For a full tutorial on the capailities
of boost::units please refer to the boost documentation.

## Some Examples
