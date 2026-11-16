#############################################################
## Header
#############################################################

FxInfo

LifeSpan 0

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	Origin
		Geom	GEO_Wepr_Grenade_1
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

Condition
	On	Kill

	Event
		EName	All
		Type	Destroy
	End
End

#############################################################

End