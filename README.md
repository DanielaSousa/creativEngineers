# creativEngineers
keywords : Motofil , Fanuc , Cooperative robot, verdinho, sunset hackathelon 2017, Rock Paper Scissors

During 72 hours we were challenged by Motofil to design and implement a robotized application that shows how secure Cooperative Fanuc robots can be.

The project was presented in Sunset Hackathelon 2017, and the main goal was to put "Verdinho" playing rock paper scissors with a person, rewarding it in the end. 

Main steps:

1 -  detect the human's move using a webcam and image processing with openCV  
2 - using an algorithm that, depending of all the moves previous played by the human, selects a gesture that is then performed by the robot using the hydraulic valves to control the gripper.  
3 - When the player stops the game by pressing the "stop" button in the Qt interface, the robot decides who wins and acts acordingly to it.  
4 - If the player won then the robot searches for an apple picking it up after and rewarding the player  
5 - In all this main steps the graphic interface is showing all the rules and  what is appening in that stage

The gripper is a very important piece because it was designed in order to be secure and to share the same concept of Cooperative robots. 

To achive this solution the project was devided in 3 main areas (as shown in the folders):

1 - "CADs" that contains the design of the grip that was  printed in a flexible material to act as a robot hand.   
2 - "Arduino" where the firmware of all hydraulic valves are.  
3 - "src" holding all the software to connect to the robot and to play the game  
4 - "others" 


We all hope to continue to have the opportunity to develop and improve this work!
