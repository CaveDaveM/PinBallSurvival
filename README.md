# PinBallSurvival

Developed with Unreal Engine 5

Final Major Project for Norwich University of the Arts

Pinball Survivor
A survivor-style game built in Unreal Engine 5 that blends physics-based pinball mechanics with goal-oriented AI. The player is a ball. bumpers, flippers, and the arena itself dictate how you move, while enemies pursue you using a GOAP planner backed by a finite state machine.
AI — GOAP Planner
Enemy decision-making is driven by a Goal-Oriented Action Planner (GOAP) rather than hand-authored behaviour trees. Each enemy holds a world state, a set of available actions with preconditions and effects, and a prioritised list of goals. At runtime, the planner uses A* to search the action space and produce the lowest-cost sequence of actions that satisfies the current goal.
This approach means:

Behaviour emerges from goals, and available actions, not rigid scripts — adding a new action automatically expands what every agent can do.
Plans replan dynamically when the world state changes (player moves, action fails, new threat appears).
Designers can tune behaviour by adjusting action costs and goal priorities rather than editing logic trees.

A finite state machine sits underneath the planner to handle execution of individual actions — moving, attacking, idling, recovering — keeping the planner focused on what to do while the FSM handles how it gets done.
Movement
Movement is fully physics-based. The player ball responds to forces, friction, and collisions rather than scripted translations. Bumpers apply impulses that increase the ball's speed on contact, creating escalating intensity as the arena fills with hazards.
Built With

Unreal Engine 5
C++

Status
In active development as part of a postgraduate dissertation project.
A 
short representation of the GOAP working.

https://github.com/user-attachments/assets/1e59f62e-cc65-4369-94c6-769d5ee98619
