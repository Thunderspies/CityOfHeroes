#########################################################
##	jack
##

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Necro_Sword_02
		Sound swordshing 100 100 .7
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		BHVR	:GlowOffset.bhvr
		Part	:NecroCore.part
		Part	:NecroCore1.part
	End

End

End