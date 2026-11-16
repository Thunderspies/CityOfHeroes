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
		Geom	Diesel_75
		Bhvr	Behaviors/Robot_fadeinandout.bhvr
		Sound PaladinBuild2_loop 80 80 .77

	End

	Event
		EName 	Pivot_base
		Type	Start 
		At	Root 
		Geom	Sprocket_base
End

	Event
		EName 	Scaffolding_50
		Type	Start 
		At	Root 
		Geom	scaffolding_50
		Bhvr	Behaviors/Robot_fadeinandout.bhvr
	End

	Event
		EName 	Scaffolding_75
		Type	Start 
		At	Root 
		Geom	scaffolding_75
		Bhvr	Behaviors/Robot_fadeinandout.bhvr
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


	Event
		EName 	bot_4
		Type	Local 
		At	Pivot_base
		Anim	Sprocket_01
		Bhvr	Behaviors/sprocket_offset_4.bhvr

	End

	Event
		EName 	bot_5
		Type	Local 
		At	Pivot_base
		Anim	Sprocket_01
		Bhvr	Behaviors/sprocket_offset_5.bhvr

	End


	Event
		EName 	bot_6
		Type	Local 
		At	Pivot_base
		Anim	Sprocket_02
		Bhvr	Behaviors/sprocket_offset_6.bhvr

	End


	Event
		EName 	bot_7
		Type	Local 
		At	Pivot_base
		Anim	Sprocket_01
		Bhvr	Behaviors/sprocket_offset_7.bhvr

	End

	Event
		EName 	bot_8
		Type	Local 
		At	Pivot_base
		Anim	Sprocket_01
		Bhvr	Behaviors/sprocket_offset_8.bhvr

	End


	Event
		EName 	bot_9
		Type	Local 
		At	Pivot_base
		Anim	Sprocket_02
		Bhvr	Behaviors/sprocket_offset_9.bhvr

	End

	Event
		EName 	bot_10
		Type	Local 
		At	Pivot_base
		Anim	Sprocket_01
		Bhvr	Behaviors/sprocket_offset_10.bhvr

	End

	Event
		EName 	bot_11
		Type	Local 
		At	Pivot_base
		Anim	Sprocket_01
		Bhvr	Behaviors/sprocket_offset_11.bhvr

	End




End

############################################################################
####### Anim States - this sets off the anim bits.                     #####
####### I'using different Conditions to offset the timing of the anims. ####
####### Use one bit for now. bug Jimb later                            #####
############################################################################

Condition
	Time 1
	Event
		EName	bot_1
		Type	SetState
		SetState "Smash Pose High"
	End
End


Condition
	Time 10
	Event
		EName	bot_2
		Type	SetState
		SetState "Smash Pose Low"
	End
End

Condition
	Time 20
	Event
		EName	bot_3
		Type	SetState
		SetState "Smash Pose"
	End
End

Condition
	Time 1
	Event
		EName	bot_4
		Type	SetState
		SetState "Smash Pose"
	End
End


Condition
	Time 10
	Event
		EName	bot_5
		Type	SetState
		SetState "Smash Pose"
	End
End

Condition
	Time 20
	Event
		EName	bot_6
		Type	SetState
		SetState "Smash Pose"
	End
End

Condition
	Time 1
	Event
		EName	bot_7
		Type	SetState
		SetState "Smash Pose Low"
	End
End


Condition
	Time 10
	Event
		EName	bot_8
		Type	SetState
		SetState "Smash Pose"
	End
End

Condition
	Time 20
	Event
		EName	bot_9
		Type	SetState
		SetState "Smash Pose High"
	End
End


Condition
	Time 1
	Event
		EName	bot_10
		Type	SetState
		SetState "Smash Pose"
	End
End


Condition
	Time 10
	Event
		EName	bot_11
		Type	SetState
		SetState "Smash Pose Low"
	End
End

Condition
	Time 20
	Event
		EName	bot_12
		Type	SetState
		SetState "Smash Pose Low"
	End
End



End


