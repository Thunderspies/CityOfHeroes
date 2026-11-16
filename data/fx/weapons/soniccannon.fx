#########################################################
##	jack
##
FxInfo

LifeSpan 0

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	WepR
		#Sound	Sonic_loop 70 60 .13
	End
End


Condition
	On 	Time
	Time 	4

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Sonic_Cannon_01
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound axeout2 50 50 .6
	End
End

End



