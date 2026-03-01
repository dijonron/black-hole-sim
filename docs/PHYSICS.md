# Physics & Math

## Newtonian Derivation

For a simple simulation, we can use Newton's law of universal gravitation,

$$
F = G\frac{mM}{r^2},
$$

to determine the radius at which light cannot escape the gravitational field of an object.

If we integrate the force, we can find the work required to move an object of mass $m$ from $R$ (the surface of the gravitational source of mass $M) to infinity (i.e. escape from the gravitational field),

$$
U = \int_R^{\inf} G\frac{mM}{r^2} dr = G\frac{mM}{R}.
$$

An object with kinetic enregy equal to this will have the minimal required velocity yo escape,

$$
\frac{mv^2}{2} = G\frac{mM}{R},\\
v = \sqrt{\frac{2GM}{R}}.
$$

For light, $v=c$, so we can solve this to find the radius $R$ at which light cannot escape from an object of mass $M$,

$$
c = \sqrt{\frac{2GM}{R}}, \\
R_S = \frac{2GM}{c^2}.
$$

## Relativistic Derivation

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

### Flat Schwarzschild Metric [2]

The simplest scenario to consider is a particle moving through spacetime curved by a massive sphere. We'll skip the deriviation here, but the Schwarzschild metric for the empty spacetime outside a spherical body of mass $M$ is

$$
ds^2 = c^2 \left( 1 - \frac{2GM}{c^2r}\right)dt^2 - \left(1 - \frac{2GM}{c^2r}\right)^{-1}dr^2 - r^2d\theta^2-r^2\sin^2\theta d\phi.
$$

Because of the spherical symmetry of the Schwarzschild metric, we can reduce the problem to that of a 2D ODE, by constraining the photons to the equitorial plane.
