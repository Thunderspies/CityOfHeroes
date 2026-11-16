#############################################################
## Robotics_Rifle.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Geom	GEO_WepR_Mastermind_Military_Rifle
		Sound shotgunout 40 40 .3
	End
End

#############################################################

End