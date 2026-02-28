# Black-Hole-Sim

A small C++ project to simulate a black hole and its accretion disk. The primary goals are to learn modern C++ and to refresh concepts from general relativity by building a simple ray-traced/particle-based simulator.

## Planned Features

- [ ] Schwarzschild metric, flat disk - Non-rotating black hole, thin disk in the equatorial plane. Gets you the basic shadow and lensing effect relatively quickly.
- [ ] Add Doppler and gravitational redshift — The side of the disk rotating toward you appears brighter and blueshifted; the other side is dimmer and redshifted.
- [ ] Kerr metric — A spinning black hole drags spacetime (frame dragging), which distorts the image further and shifts the innermost stable circular orbit (ISCO).
- [ ] Disk emission model — Instead of a flat color, give the disk a temperature profile (hotter near the center) and map it to a blackbody color.
- [ ] Multi-order images — Rays that loop around the black hole multiple times produce faint "ghost" images of the disk stacked inside the main image.

## Requirements

- TODO

## Instructions

- TODO

## Physics & Math

### Flat Spacetime [1]

To measure the distance across a path, $P$, we can use a idfferential formula called a **metric**,

$$
(dl)^2 = (dx)^2 + (dy)^2 + (dz)^2 .
$$

We can integrate the metric along $P$ to calculate the total distance,

$$
\Delta l = \int_{1}^{2} \sqrt{(dl)^2} =  \int_{1}^{2} \sqrt{(dx)^2 + (dy)^2 + (dz)^2 }.
$$

The shortest distance between two points in flat space is measured along a straight line; we can define the "straightest possible line" between two points as the path where $\Delta l$ is a _minimum_.

We can similarly use the **metric for flat spacetime**,

$$
(ds)^2 = (cdt)^2 - (dl)^2 = (cdt)^2 - (dx)^2 - (dy)^2 - (dz)^2,
$$

to measure the interval along a worldline, $W$,

$$
\Delta s = \int_{A}^{B} \sqrt{(ds)^2} = \int_{A}^{B} \sqrt{(cdt)^2 - (dx)^2 - (dy)^2 - (dz)^2}.
$$

### Curved Spacetime [1]

In a curved spacetime, the "straightest possible worldline" will be curved (given that mass warps spacetime). In curved geometry, the straightest possible lines are called **geodesics**. In flat spacetime, geodesics _are_ straight.

Three fundamental features of general relativity are:

- mass acts on spacetime, telling it how to curve,
- spacetime acts on mass, telling it how to move, and
- freefalling particles (including photons) follow geodesics through spacetime. For massive particles, the geodesic has a _extreme_ (max or min) interval, while light has a _null_ interval $(\int\sqrt{(ds)^2} = 0)$.

For convinence, situations with spherical symmetry can use spherical coordinates ($r, \theta, \phi$). This means we can rewrite the metric as

$$
(dl)^2 = (dr)^2 + (rd\theta)^2 + (r\sin{\theta} d\phi)^2.
$$

#### Schwarzschild Metric

The simplest scenario to consider is a particle moving through spacetime curved by a massive sphere.

## References

- [1] [An Introduction To Modern Astrophysics](https://www.indigo.ca/en-ca/an-introduction-to-modern-astrophysics/9781108422161.html)
- [2] [General Relativity: An Introduction for Physicists](https://ia601900.us.archive.org/8/items/gnsecBooks/%5BM._P._Hobson%2C_G._P._Efstathiou%2C_A._N._Lasenby%5D_Ge%28BookFi%29.pdf)
- [3] [Gravitational Lensing by Spinning Black Holes in
  Astrophysics, and in the Movie Interstellar](https://arxiv.org/pdf/1502.03808)

## License

- See the repository `LICENSE` for licensing details.
