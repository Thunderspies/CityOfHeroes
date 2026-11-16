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
		Geom	GEO_WepL_Ringblade_03
		Sound swordshing 100 100 .7
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

End