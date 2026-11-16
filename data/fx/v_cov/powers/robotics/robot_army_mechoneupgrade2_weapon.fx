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
		#Sound	Glow3_loop 80 30 .26
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
		Geom	FX_Soldier_Laser_Auto
		
	
	End
	
	Event	
		Type	local
		At	chest

		ChildFx		V_COV/Mastermind_pets/Robotics/RobotEyeGLow_Soldier.fx
		
	
	End

	Event	
		Type	local
		At	back

		BHVR	:LeftLightOffset.bhvr
		Part	:RobotLightGlows.part
		Part	:RobotLightStar.part
		
		
		
	
	End

End

End		