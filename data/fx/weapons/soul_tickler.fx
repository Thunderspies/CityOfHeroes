#########################################################
##	jack
##
FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	4

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	Soul_Tickler
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound shotgunout 50 50 .3
	End
End

End



