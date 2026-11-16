#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On	Time
	Time	9

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\Weapon_Fade_UP.bhvr
		Geom	Arachnos_Mace03
		Sound MaceOut 70 70 .44
	End
End

#############################################################

End