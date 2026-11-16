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
		EName 	Prime
		Type	Local
		At	WepR
		Bhvr	Behaviors\Weapon_Fade_UP.bhvr
		Geom	GEO_WepR_Necro_Sword_02
		Sound swordshing 100 100 .7
	End
End

#############################################################

End