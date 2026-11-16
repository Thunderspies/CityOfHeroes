#########################################################
##	jack
##

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	Coral_Hammer
		Bhvr	Behaviors/Coral_Weapon_Fade_UP.bhvr
		Sound maceout 60 60 .5
	End
End

End