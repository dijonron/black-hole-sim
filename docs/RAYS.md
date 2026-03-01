# Ray Tracing

## Flat Schwarzschild

A ray is defined as the points you can reach by travelling straight from the origin in some direction for a distance $t$: $P(t) = origin + t * direction$. We want to find where (if anywhere) it intersects our accretion disk or event horizon.

### Event Horizon Hit

The event horizion is a sphere of radius $R_S$, so we want to find a value of $t$ where that point lands exactly on the sphere,

$$

\|P(t)\| = R_S,
\\
\|origin + t * direction|^2 = R_S^2,
\\
(origin\cdot origin) + 2t(origin\cdot dir) + t^2(dir \cdot dir) = R_S^2.
$$

We can simplify this to

$$
at^2 + bt + c = 0
$$

where

$$
\begin{align*}
a &= dir \cdot dir,\\
b &= 2(origin\cdot dir), \\
c &= (origin\cdot origin) - R_S^2.
\end{align*}
$$

The solution to this is

$$
t = \frac{-b \pm \sqrt{b^2-4ac}}{2a}.
$$

If the discriminant is $<0$, no real solutions exis, thus no intersection. If it is $0$ the ray is a tangent to the sphere. If it's positive, we have two real solutions (entry and exit of the sphere). Since we only care about the first intersection, we can solve only for the nearest solution,

$$
t = \frac{-b - \sqrt{b^2-4ac}}{2a}.
$$

### Accretion Disk Hit

The disk lies in the xz plane ($y=0$). We need to find $t$ where the ray crosses $y=0$.

If the ray's $y$ component is nil, there will be no interesction (the ray is parallel to disk plane).

$$
origin_y+t∗dir_y = 0, \\
t = -origin_y /dir_y
$$

## Schwarzschild Metric

Instead of tracing the rays along a straight line, we will shoot rays from the camera and integrate their path through the curved spacetime. If the path falls into the event horizon, it will be black. If it hits the accretion disk, it will be coloured. If it escapes to infinity, it will be a background pixel.
