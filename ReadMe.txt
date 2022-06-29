Requirements for build:

Windows SDK version: 10.0.17763
Platform Toolset: Visual Studio 2017 (v141)
C++ Language Standard: C++17

A Bézier curve is a parametric curve used in computer graphics and related fields. A set of discrete "control points" defines a smooth, continuous curve by means of a formula. 
This tool provides switch calculation between two Bézier curves.

Steps for calculation:
1. Set up first curve by input of formula or reading from file (example: "file1.txt").
f(t)=3*t*(1-t)^2*P1

- no spaces
- multipliers are explicit
- square spelled like "^"

2. Write points of curve in format: (x;y;z)

3.Set up second curve
4. Enter "Calculate Minimal Distance"