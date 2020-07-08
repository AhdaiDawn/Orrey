# Orrey
A solar system  written for Vulkan in C++,base on QWindow.

环境：
    - Vulkan SDK 1.2.135.0
    - Qt 5.12.9 (MSVC 2017 64-bit)
    - Visual Studio 2019

该项目的主要目的是在计算着色器上实现实时重力计算，同时通过一次绘制调用绘制所有对象。

该项目实现的功能包括计算着色器(compute shaders)，实例化(instancing)，纹理数组(texture arrays)，mipmapping，uniform 和 storage buffers，多重采样(multisampling)，专业化常量(specialization constants)，查询池(query pools)等。

距离以天文单位缩小。质量是太阳质量。太阳的大小不成比例，并且已按比例缩小以使观察行星更容易。月亮距离也被夸大了。