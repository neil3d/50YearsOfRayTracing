# 光线追踪简史 

用代码叙述光线追踪技术对发展历程

- 回看光线追踪的发展，将其中的一些关键对推进作用对文章找出来，并用代码实现
- 提供尽量简单，直观的 CPU 算法实现；代码的简单/易理解为首位，不太考虑中性能优化
- 使用现代 C++ 编程

## 文章列表

| 文件夹  |  作者    |  文章标题    |
| --  |  --  |  --  |
|  1968.Appel   | Arthur Appel |  Some techniques for shading machine renderings of solids    |
|  1979.Whitted | Turner Whitted  |  An improved illumination model for shaded display   |
|  1984.Cook    | Robert L. Cook, Thomas Porter, Loren Carpenter    | Distributed ray tracing  |
|  1986.Kajiya  | James T. Kajiya | The rendering equation  |

## 工程配置

- 安装 vcpkg 及依赖库
- VSCode 配置

## Ray Casting - Arthur Appel, 1968

- 具体代码可见子目录：**1968.Appel**

![Screen Shot](./1968.Appel/images/Screen-Shot-2020-05-08.png)

- 算法描述： TODO

![diagram](./1968.Appel/images/wiki_ray_trace_diagram.svg)
> 图片来自维基百科

## Recursive Ray Tracing - Turner Whitted, 1980
