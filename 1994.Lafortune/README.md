# Modified Phong for Physically Based Rendering

- Original Phong reflection model 
    - https://dl.acm.org/doi/10.1145/360825.360839

- 天妒英才：Phong模型作者--裴祥风(Bui Tuong Phong)  
![portrait](./images/bui-tuong-phong.jpg)
    - 1942年12月出生于越南，1973年取得博士学位，博士论文的内容大家都很熟悉。1975死于白血病。
    - 他和Ed Catmull是师兄弟，同样师从 Ivan Sutherland（为图形学第一次斩获图灵奖）。
    - see: [University of Utah - History of the School of Computing](https://www.cs.utah.edu/about/history/)

- Modified Phong
    - for energy conservation: $\frac{s+2}{2\pi}$ , $K_d+K_s \leq 1$