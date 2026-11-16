#############################################################
## Header
#############################################################

FxInfo

LifeSpan 0

Flags InheritAlpha DontSuppress IsWeapon

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	Origin
		Geom	GEO_Wepr_Bow_1
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

End