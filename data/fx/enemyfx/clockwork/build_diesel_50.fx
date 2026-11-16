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
		Geom	Diesel_50
		Bhvr	Behaviors/Robot_fadeinandout.bhvr
		Sound PaladinBuild2b_loop 80 80 .67

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
		EName 	bot_12
		Type	Local 
		At	Pivot_base
		Anim	Sprocket_02
		Bhvr	Behaviors/sprocket_offset_12.bhvr

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
		SetState "Smash Pose High"
	End
End


Condition
	Time 10
	Event
		EName	bot_5
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


