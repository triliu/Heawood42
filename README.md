# Heawood42

The Heawood42 is a diodeless 42-key ortho-linear split keyboard that uses any pro micro microcontroller. The Heawood42 utilises a new type of circuit design stemming from graph theory and mathematical design theory. All previous diodeless keyboards work using one pin per key, such as the [sweep](https://github.com/davidphilipbarr/Sweep), which uses 17 pins to control 17 keys per side, the [sweeeeep](https://fingerpunch.xyz/product/sweeeeep/), which uses 17 pins to control 17 keys per side, the [cantor](https://github.com/diepala/cantor), which uses 18 pins to control 18 keys per side, and the [piantor](https://github.com/beekeeb/piantor), which uses 21 pins to control 21 keys per side. The first two keyboards can use any pro micro-style board (traditionally, the pro micro-style boards have 18 pins available, but one of these pins is required for communication among the halves). The third and fourth keyboard requires the use of a larger microcontroller, the BlackPills and the Raspberry Pi Pico, respectively. 

The Heawood42 is the first split diodeless keyboard requiring fewer pins than keys. Each side uses 14 pins to control 21 keys. This means that even on a typical pro micro-style board, the Heawood42 may have inter-board communication, addressable RGB lights, and an OLED screen. 

Due to its diodeless-ness, the Heawood42 is ideal in several cases, as it reduces the cost and complexity of making a custom mechanical keyboard.

<img src="https://github.com/triliu/Heawood42/assets/3928134/d9548d26-492f-40ff-b97c-e8ae36825b7c" width="380">
<img src="https://github.com/triliu/Heawood42/assets/3928134/e1ea87da-0814-44ba-95fe-ba899eff5412" width="380">



## Why 'Heawood'?

Typical mechanical keyboards arrange their keys in a grid pattern. In such a grid, there are cycles of four keys. If no diodes were used and three keys of a four-key cycle were pressed, the keyboard would act like all four keys were being pressed. See https://pcbheaven.com/wikipages/How_Key_Matrices_Works/ for an explainer of this phenomenon. By adding a diode to every key, the keyboard becomes "n-key rollover", meaning that the keyboard can theoretically distinguish all combinations of keypresses. In software, however, 6-key rollover is typically used, meaning the software assumes that if more than six keys are being pressed, then keys were pressed on accident. The USB keyboard protocol is limited to a 6-key rollover. For split keyboards, a 6-key rollover on each half is excessive, as typically one finger would only press one key at a time, and even a 5-key rollover is more than is needed. 

In graph theory, it is possible to make graphs that do not contain small cycles. Each graph can be converted into a new type of key matrix, with differing characteristics. The Heawood graph is a graph that is the incidence graph of a projective plane of order 2, and has 14 vertices, 21 edges, and a girth of 6. For more details on this graph, see [House of Graphs](https://houseofgraphs.org/graphs/1154). This graph converts to a key matrix with just a 4-key rollover. However, by designing the board specifically, all problematic 5-key and 6-key sequences may be practically eliminated, yielding the Heawood42. 

<img src="https://github.com/triliu/Heawood42/assets/3928134/4ef2aa62-9b1a-47e1-b93b-3c5a06f6acfd" width="190">


This design is powered using any pro micro-style microcontroller.

## What files are here

- Gerber files for production
- QMK firmware
- 3D files for the case

