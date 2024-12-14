## Project title

Pi Invaders - Big Game Edition

## Team members

1. Ahmad Zafar
2. Hadil Owda

## Project description

We developed a modified version of the famous 'Space Invaders' game making it Stanford-Cal Big Game themed. The game works in a way that there are Cal Bears as the space invaders who are shooting C's at you. You being the 'Cardinal Tree' have to avoid them and At the same time shoot the Bears back with your '+' bullets. There are two scores in the game simultaneously just like real Football. Every time you get hit by a 'C', Berkeley scores. Every time you hit a Bear, Stanford's score increases. You have 30 seconds and you have to shoot more Bears than the C's that hit you to ultimately win the game, and break Stanford's losing streak. 

## Member contribution:

Ahmad Zafar: 
- Implementation of interrupts in the game loop
- Bullets Graphics (shooter to bears & bears to shooter)
- Collisions and Game logic + their Graphics (scoring/timer/game loop/game state screens)


Hadil Owda:
- Bears & Shooter graphics (sprite creation and movement - including initial animation for the initial bugs) 
- Optimized Graphic Clearance for Bears & Shooter (specific clearance only at changed coordinates to reduce overhead)
- Sounds Module Implementation (using a buzzer) 

Both:
- The hardest challenges were to speed up the graphics rendering as the space invaders game works in a way that the whole grid of enemies keeps moving down in a zig-zag way which had a very high cost on the graphics rendering as it took a lot of time to clear and redraw again and again. Initially, we thought the interrupts were the problem so hadil started off by implementing a keyboard timeout function which helped us speed up a bit as the previous keyboard implementation in assignment was blocking. However, the keyboard time out was not processing more than one key per iteration of a loop, so then Ahmad worked on getting the interrupts working by draining the scancodes from the ringbuffer directly from ps2. this helped make it a bit faster, but the main problem wasn't solved yet. So both of tried to work on implementing 4 framebuffers, but unfortunately we found out that we can't do that on Mango Pi as it supports on active fb at time based on the de.h module header file. Then, we optimized what we had and made sure to clear graphics only where needed at the specific coordinate. That improved the speed by a huge margin, however it still wasn't optimal.  Then, instead of the enemies moving down, we had a unique solution to the problem which was to move the shooter up at specific time intervals rather than the whole enemy grid, which basically achieves the same effect. 

## References:

For the pre-computer sprites of the enemies and shooter graphics, we used a website to pixelate image. Then we used a python script online, tweaked it a bit and ran it on our local code compiler to provide us with a sprite which is a 2D array of all the pixels in the photo we allowed us to go over them with the draw pixels function we implemented earlier. Here are the link we used:

https://stackoverflow.com/questions/70149734/how-to-convert-an-image-into-an-array-of-its-raw-pixel-data

How to convert an image into an array of its raw pixel data?
You can do it with Pillow (PIL) library:. from PIL import Image import numpy as np path = 'image.png' image = Image.open(path) image = np.asarray(image) Notice that returned arrays have a length of 4 because the ".png" format supports an additional Alpha channel (transparency), so it's RGBA.
stackoverflow.com

https://giventofly.github.io/pixelit/



## Self-evaluation
How well was your team able to execute on the plan in your proposal?  
Any trying or heroic moments you would like to share? Of what are you particularly proud:
the effort you put into it? the end product? the process you followed?
what you learned along the way? Tell us about it!

Even though we ended up implementing something different from our original proposal for the final product, we are proud of how we pivoted and creatively developed a working solution. While we technically implemented our initial idea, it was too slow, and we wanted a final project that actually worked and was fun to play. We're proud of how we worked around these challenges to create a final project that was fast enough to be enjoyable.
For us, this was a heroic moment - we saved the day and our final demo! When we faced a major technical bottleneck with the graphics rendering for the moving bears, we came up with a creative solution. Instead of trying to optimize the bears' movement, which wasn't working despite our best efforts, we flipped the concept and made the shooter move upward instead. This clever twist achieved the same exciting gameplay feel as Space Invaders while bypassing our rendering limitations entirely.
We learned the importance of teamwork and being creative in solving issues. We also learned not to be afraid to pivot from our original plans, as we might end up with an even better project! This was a fun class filled with challenges that we're proud to have overcome.

## Photos
You are encouraged to submit photos/videos of your project in action. 
Add the files and commit to your project repository to include along with your submission.
