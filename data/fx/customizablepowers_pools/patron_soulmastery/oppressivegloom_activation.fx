#############################################################
## OppressiveGloom_Activation.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At 	origin
		Sound chargeup4 60 60 .66
	End
End

Condition
	On	Time
	Time	28
	Event
		Ename	Logo
		Type	Local
		At	Chest
		Part	:Logo01.part
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	root
		Part1	CustomizeablePowers\DarkArmor\DarkArmorRing.Part
		Sound darkarmor2 60 60 0.75
	End
End

#############################################################

Condition
	On	Time
	Time	50

	Event
		EName	RINGS
		Type	Local
		At	chest
		part	CustomizeablePowers\DarkArmor\RedHitMist.part
		part1	CustomizeablePowers\DarkArmor\RegenerationRinga.part
		part1	CustomizeablePowers\DarkArmor\RegenerationRing_Tinted.part
	End
End

Condition
	On	Time
	Time	60

	Event
		Type	Local
		At	FootR
		Sound miasma_loop 60 60 0.6
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 400
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr	Behaviors\soundFade3.bhvr
		Sound glow3_loop 60 60 0.4
		Flags	PowerLoopingSound
		Lifespan 400
	End

	Event
		Type	Local
		At	LLEGL
		Sound gloomy_downer 60 60 0.79
	End

	Event
		Type	Local
		At	FootL
		Sound rage 100 100 1.0
	End
End

#############################################################

End


