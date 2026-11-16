#########################################################
## Rikti Sword 3
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	WepR
		Sound axeout 100 100 .67
	End
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Geom	GEO_WepR_Rikti_Sword_03
		Sound swordshing 100 100 .5
	End

	Event
		EName 	Geomhookup1
		Type	Local
		At	WepR
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Geom	RiktiBlade03
		Sound glow8_loop 60 60 .55
	End
End

#########################################################

End