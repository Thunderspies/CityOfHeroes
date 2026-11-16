#############################################################
## baseball_bat.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName Prime
		Type	Local
		At	WepR
		Until	RIGHTHAND #If a Righthandweapon is visible, don't draw the bat
		Geom	GEO_WepR_BaseBallBat
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

#############################################################

End