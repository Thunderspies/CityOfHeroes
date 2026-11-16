#########################################################
##	template

FxInfo


Flags    InheritAlpha

#########################################################
#################### FX ########################
#########################################################

Condition
	
	Event	
		Type	local
		At	LarmR
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Geom	FX_Soldier_Laser_Light
		
	
	End

	Event	
		Type	local
		At	LarmL
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Geom	FX_Hammer
		
	
	End
	
End

End		