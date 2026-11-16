#########################################################
##	
########################################################
FxInfo

Flags InheritAlpha

#######  Left  ####################################################

Condition
	Event
		EName 	Anim_Prop
		Type	Local
		At	LarmL 
		Anim	FX_Robotic_Fist_L
		Bhvr	Behaviors/RoboticsWeapon_Fade_UP.bhvr
	End
End

##### Right #######################################################

Condition
	On	Time
	Time	0
	Event
		EName 	Anim_Prop
		Type	Local
		At	LarmR 
		Anim	FX_Robotic_Fist_R
		Bhvr	Behaviors/RoboticsWeapon_Fade_UP.bhvr
	End


End

End

