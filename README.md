# Pong-mbed
Last updated: 17/08-2020<br/><br/>

<b>Introduction:</b><br/>
I made Pong for the STM32F7-DISCO EC

<b>Features:</b><br/>
Pong is a 2 player PvP game, in which you each have a bar, which you can use to keep a ball from going behind you.<br/>
If the ball does get behind you the opposing player gets a point. When you decide to stop playing, the player with the most points win.<br/>
When you start the game you also have the choice to increase the speed and FPS. The FPS changes how fast the ball moves and the speed changes how many pixels the ball moves each frame.<br/>
The speed will also exponentially increase automatically if no player has scored for some time.<br/>
There is also an option to turn on or off sound.<br/>
Each player is controlled with a potentiometer for full analog controls.<br/>
There is also 3 usable buttons, 2 of which is integrated on the board.<br/>
The black button takes you back to the menu.<br/>
The blue button pauses/unpauses the game. And the third button connected with a cable resets the score and ball position, it also clears any artifacts if the game has glithed. (Has not happened to me)<br/> 


<b>How I made it:</b><br/>
I made it by drawing a picture on the screen many times a second, which makes it look like the objects move.



<b>Problems/Changes:</b><br/>
Originally I cleared the entire screen each frame, but the screen could not update that fast, so instead I just cleared the pixels that need be cleared.


<b>Libraries used:</b><br/>
<a href="https://github.com/ARMmbed/mbed-os.git">mbed.h</a> - mbed 0S 6.2.0 library<br/>
rect.h - header for custom class<br/>
<a href="https://os.mbed.com/teams/ST/code/BSP_DISCO_F746NG/">stm32746g_discovery_lcd.h</a> - library for the visual part of the display<br/> 
<a href="https://os.mbed.com/teams/ST/code/BSP_DISCO_F746NG/">stm32746g_discovery_ts.h</a> - library for the touch part of the display<br/>
vector - library for using vectors


<b>Pin Connections:</b><br/>
Devices have to be connected to the following pins<br/>
Button: D4<br/>
Potentiometer 1: A0<br/>
Potentiometer 2: A1<br/>
Buzzer: D8<br/>

<br/>
<b>Made by Troels Wittrup-Jensen</b>
