#########################################################
##	template

FxInfo


Flags    InheritAlpha
#LifeSpan		300
#########################################################
####################### AUDIO ###########################
#########################################################



Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound robotupgrade_loop 25 25 .15
	End
End


#########################################################
#################### FX ########################
#########################################################

Condition
	On	Time
	Time	124
	
	Event	
		Type	local
		At	LarmR
		Bhvr	Behaviors/RoboticsWeaponUp_Fade_UP.bhvr
		Geom	FX_Soldier_Heavy_Laser
		
	
	End
		
	Event	
		Type	local
		At	back

		BHVR	:LeftLightOffseta.bhvr
		Part	:RobotLightGlows.part
		Part	:RobotLightStar.part
		
		
		
	
	End

	Event	
		Type	local
		At	back

		BHVR	:RightLightOffseta.bhvr
		Part	:RobotLightGlows.part
		Part	:RobotLightStar.part
		
		
		
	
	End
End

End		