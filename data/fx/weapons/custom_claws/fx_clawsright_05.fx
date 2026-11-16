#########################################################
## FX Geo - Right Claws - 05
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	Origin
		#Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Geom	GEO_WepR_Claw_02
	End
End

#########################################################

End