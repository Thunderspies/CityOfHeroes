#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	hookup
		Type	Local
		At	wepR
		BHVR	Behaviors/Weapon_Fade_UP.bhvr
		Geom	GEO_WepR_Divining_Staff
		Sound DiviningRodOut 50 50 .65
	End

	Event
		Ename	Sparkle
		Type	Local
		At	hookup
		Geom	Staff_06
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

Condition
	On 	cycle
	Time 	15
	Chance	.3

	Event
		Type	start
		At	Sparkle
		altpiv	1
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part	:Shine.part
		Part	:Shinering.part
		LifeSpan	10
	End
End

#############################################################

End