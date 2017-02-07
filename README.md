# NBodySimulator

Welcome to the N-Body Gravitational Simulator.

This simulator simulates the gravtational forces on n bodies using the euler method for solving differential equations.

Way to compile:-

I used the CodeBlocks IDE, Win32 api and OpenGL for this project. It is recommended that the project be compiled through CodeBlocks. To compile some libraries must be included => libopengl32.a, libwinmm.a, libcomdlg32.a.
These libraries can be found with the default package of CodeBlocks(MinGW).

Resources have been generated through ResEdit.

Some more help:-

You can Place Physical Bodies in the simulator(The Big Black-ish Area) by pressing the left mouse button. If you have pressed the LMB and not released it yet then you can cancel the place operation by double clicking the right mouse button. If you right click and drag the RMB then, the sizer field in the toolbar will be populated with the coordinates of the mouse position.
The initial velocity of the body will be set by calculating the length of the mouse line(when you left click an drag the mouse).

Note:- This simulator supports schwarzschild blackholes by calculating the schwarzschild radius(Of course none of it is accurate as it does not take relativity into account which becomes apparent for higher masses). Also some stars....
Another thing to note is that when the units of the simulator are changed the radius of bodies also change. I know it is bad this way but I did not change it and now I sort of like it this way (XD)...

The layout:-

The tool window is the heart of this program because the user can control this simulator (mostly) with the help of it.

First is the Reset button.
This button will reset the simulation.
The second button is Pause.
This button can be used to pause and unpause the simulator.
The next is the Trajectory checkbox.
NOTE:-You can use the trajectory feature but it is recomended to be used when the simulator is paused.(When paused the simulator will show predictive trajectories of the
particles if the Trajectories checkbox is checked. When unpaused it would show realtime(dynamic) predictive - only when the LMB remains pressed - trajectory of the same but beware: the more the no. of particles, the more expensive the trajectory operation.)
The next is the collision type. If you choose destroy then bodies will destroy after colliding. If you choose merge then bodies will merge after colliding.
The merged bodies will have a new colour proportional to their masses and the momentum of the mearger is conserved. If you choose merge and destroy then same mass bodies will be destroyed and others will merge.

The next is the units dropdown. Here you can set the units of the simulator. Available units are:-
1nm = 10^-9m
1mim = 10^-6m
1mm = 10^-3m
1cm = 10^-2m
1 km = 1000m
1 ld(lunar distance) = 384402000m
1 au(astronomical unit) = 1.496e+11m
1 ly(light year) = 9.461e+15m
1 pa(parsec) = 3.086e+16m

The next section contains the body settings. Bodies can be placed by clicking the left mouse button. The mass of the body can be a minimum of 1.0 kgs and maximum of 1^40 kgs.
The radius can be a minimum of 5.0 units and maximum of 200.0 units.
These boxes, currently only support integers(no decimal points)
The example of inputting mass and radius is given underneath the input fields of the toolbar.

The next section contains the colour settings for a colourful simulation expierience. RED, GREEN and BLUE sliders - These colours can be used to form any colour.

You can save and load a simulation from the File menu options save and load.
The exit option ---- do I need to tell what it will do (XD)

The toolbar option in the window menu will hide and show the toolbar.

The music is from dl-sounds.com

The General help menu contains the same information (mostly copied from above :P)