## Physics & Math

### Flat Spacetime [1]

To measure the distance across a path, $P$, we can use a differential formula called a **metric**,

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

For convenience, situations with spherical symmetry can use spherical coordinates ($r, \theta, \phi$). This means we can rewrite the metric as

$$
(dl)^2 = (dr)^2 + (rd\theta)^2 + (r\sin{\theta} d\phi)^2.
$$

### Flat Schwarzschild Metric

The simplest scenario to consider is a particle moving through spacetime curved by a massive sphere.

(Expanded Schwarzschild equations and geodesic examples can be added here as you implement numeric integrators and ray tracing.)
