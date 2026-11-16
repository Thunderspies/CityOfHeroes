#########################################################
##	tenticle fun yea!
########################################################
FxInfo

Input  
	Inpname	Root
End


##################################################################

Condition
	On	Time
	Time	0




	Event
		EName 	Robot
		Type	Start 
		At	Root 
		Geom	Diesel_25
		Bhvr	Behaviors/Robot_fadeinandout.bhvr
		Sound PaladinBuild1b_loop 80 80 .72

	End

	Event
		EName 	Pivot_base
		Type	Start 
		At	Root 
		Geom	Sprocket_base
End

	Event
		EName 	bot_1
		Type	Local 
		At	Pivot_base
		Anim	Sprocket_01
		Bhvr	Behaviors/sprocket_offset_1.bhvr

	End

	Event
		EName 	bot_2
		Type	Local 
		At	Pivot_base
		Anim	Sprocket_01
		Bhvr	Behaviors/sprocket_offset_2.bhvr

	End


	Event
		EName 	bot_3
		Type	Local 
		At	Pivot_base
		Anim	Sprocket_02
		Bhvr	Behaviors/sprocket_offset_3.bhvr

	End


End

############################################################################
####### Anim States - this sets off the anim bits.                     #####
####### I'using different Conditions to offset the timing of the anims. ####
####### Use one bit for now. bug Jimb later                            #####
############################################################################

Condition
	On Cycle
	Time 1
	Event
		EName	bot_1
		Type	SetState
		SetState "Smash Pose Low"
		LIFESPAN 5
	End
End


Condition
	On Cycle
	Time 10
	Event
		EName	bot_2
		Type	SetState
		SetState "Smash Pose Low"
		LIFESPAN 5
	End
End

Condition
	On Cycle
	Time 20
	Event
		EName	bot_3
		Type	SetState
		SetState "Smash Pose"
		LIFESPAN 51
	End
End




End


