#########################################################
## FX Geo - Right Claws - 01
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		#Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Geom	VillianClawsR
	End
End

#########################################################

End