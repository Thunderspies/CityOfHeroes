#########################################################
##	jack
##
FxInfo

Flags InheritAlpha DontSuppress


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Bonesaw
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound MaceOut 100 100 .6
	End
End

End