#########################################################
##	
########################################################
FxInfo

Flags InheritAlpha



Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound robotupgrade_loop 25 25 .15
	End
End






#######  Left  ####################################################

Condition
	On	Time
	Time	129
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
	Time	213
	Event
		EName 	Anim_Prop2
		Type	Local
		At	LarmR 
		Anim	FX_Robotic_Fist_R
		Bhvr	Behaviors/RoboticsWeapon_Fade_UP.bhvr
	End


End

Condition
	On	Time
	Time	124

	Event	
		Type	local
		At	back

		BHVR	:LeftLightOffsetb.bhvr
		Part	:RobotLightGlows.part
		Part	:RobotLightStar.part
		
		
		
	
	End

	Event	
		Type	local
		At	back

		BHVR	:RightLightOffsetb.bhvr
		Part	:RobotLightGlows.part
		Part	:RobotLightStar.part
		
		
	End

End
	
End

