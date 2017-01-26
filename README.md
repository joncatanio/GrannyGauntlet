Reed Garmsen
CPE 476 - "Brave" Section
January 24, 2017

All features working to the best of my knowledge!

I originally developed this working on linux for 471 but ported it over to windows with Visual Studio
Therefore, running this on a non-linux machine is likely to be non-functional (and require some setup
even on windows).  However, as discussed in-class this is currently OK.

** Engine Specifics: **
+ Game camera is unchanged from 471 (other than being time-based for movement)
+ Collisions are accurate, generally one object (bunny) will bounce away from the other depending on ordering, but both may bounce depending on proximity
+ Mesh movement is time-based using a delta of 1/60th
+ Mesh orientation is accurate and consistent depending on initial direction (which is random)
+ Mesh shading and game look is simple but accurate (walls are meant to be shaded like obsidian, bunnies brass/jade and the floor is an off-green shader). All use normal phong shading
+ General game stats (fps, bunnies hit, bunnies on the field) are printed every ~1 second to the console (no UI in-game)
+ Software design is generally component-based with some remaining global data and OO design (e.g. shader logic will be refactored for 25% shader manager if my code is used by team)
