#############################################################
## HEADER
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
		Bhvr	Behaviors\Weapon_Fade_UP.bhvr
		Geom	GEO_WepR_Arachnos_Mace_02
		Sound MaceOut 70 70 .44
	End
End

#############################################################

End