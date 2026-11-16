#############################################################
## RainOfFireHands.fx
#############################################################

FxInfo

Lifespan 70

#############################################################

Condition
	On	Time
	Time	20

	Event
		EName	TestSphere
		Type	local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			Scale		2.0 2.0 2.0
			EndScale	3.0 3.0 3.0
			Alpha		192
		End
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part1	:FireHandz.part
		Part	:FireHandz_Alpha.part
		Sound Firehand_loop 100.0 100.0 .45
		LifeSpan 60
	End

	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:FireHandz.part
		Part	:FireHandz_Alpha.part
		Sound Ignite2 100.0 100.0 .8
		LifeSpan 60
	End
End

#############################################################

End