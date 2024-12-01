# GLUTAGA

## Story
In the distant future, humanity's last hope rests with you - the pilot of the experimental GLUTAGA starfighter. Earth's resources have been depleted, and mysterious alien forces guard the path to habitable worlds. As Earth's most skilled pilot, you must navigate through waves of alien defenders, clearing the way for humanity's survival ships that follow in your wake.

The GLUTAGA starfighter, powered by revolutionary OpenGL technology, represents the pinnacle of human engineering. Its unique design allows it to harness energy from defeated enemies, converting their essence into powerful plasma projectiles.

## Game Mechanics
- Control your GLUTAGA starfighter through space
- Shoot down alien enemies to score points
- Avoid enemy projectiles
- Health system with visual feedback
- Dynamic background with parallax stars and planets
- Particle effects for explosions and damage
- High score system that persists between sessions
- Game over screen with restart capability

## Controls
- **Movement**: 
  - WASD keys or Arrow keys
  - W/↑ : Move Up
  - S/↓ : Move Down
  - A/← : Move Left
  - D/→ : Move Right

- **Combat**:
  - Left Mouse Button: Shoot
  - Mouse Wheel: Adjust enemy size (debug feature)

- **System**:
  - R: Restart game after death
  - ESC: Exit game

## Installation & Running

### Prerequisites
- Dev-C++ IDE
- GLUT and GLEW libraries

### Setup Instructions
1. Download and extract the GLUTAGA game files
2. Install Dev-C++ if you haven't already
3. Open the `GLUTAGA.dev` project file in Dev-C++
4. Adjust library paths if necessary:
   - Click the "Project" tab
   - Select "Project Options"
   - Go to "Parameters" tab
   - Under "Linker", ensure the following paths are correct:
     - -lfreeglut
     - -lopengl32
     - -lglu32
   - Go to "Directories" tab
   - Under "Library", confirm that the following paths are correct:
     - C:\freeglut\lib\x64
     - C:\glew-2.1.0\lib
   - Under "Include", confirm that the following paths are correct:
     - C:\freeglut\include
     - C:\glew-2.1.0\include
   - Note: If you've installed the libraries in a different directory, you'll need to change the paths accordingly.
5. Compile and run the project (F11)

### Troubleshooting
If you encounter missing library errors:
1. Ensure GLUT and GLEW are properly installed
2. Verify that the library paths in the project settings match your actual installation paths
3. Make sure all required DLL files are in your system PATH or in the game's directory

## Credits
GLUTAGA was developed by:
- Dane Quintano
- Jansen Moral
- JC Paglinawan
- Dharmveer Sandhu

of TN34

Of course, this wouldn't be possible without the guidance of our professor, Sir Reginald Cheng. Thank you po!