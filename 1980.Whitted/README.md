# Recursive Ray Tracing - Turner Whitted, 1980

## 原文摘要

### Conventional Models

$$
I = I_a + k_d \sum_{j=1}^{j=ls} (\vec{N} \cdot \vec{L_j}) + k_s \sum_{j=1}^{j=ls} (\vec{N} \cdot \vec{\acute{L_j}})^n
$$

where
- $I$ = the reflected intensity
- $I_a$ = reflection due to ambient light
- $k_d$ = diffuse reflection constant
- $\vec{N}$ = unit surface normal
- $\vec{L_j}$ = the vector in the direction of the jth light source
- $k_s$ = the specular reflection coefficient
- $\vec{\acute{L_j}}$ = the vector in the direction halfway between the viewer and the jth light source
- $n$ = an exponent that depends on the glossiness of the surface.

### Improved Model

$$
I = I_a + k_d \sum_{j=1}^{j=ls} (\vec{N} \cdot \vec{L_j}) + k_s S + k_t T
$$

where
- $S$ = the intensity of light incident from the $\vec{R}$ direction
- $k_t$ = the transmission coefficient
- $T$ = the intensity of light from the $\vec{P}$ direction.

![Figure 1](./images/figure_01.png)

### Visible Surface Processor

- bounding volume
- anti-aliasing

![Figure 5](./images/figure_05.png)
