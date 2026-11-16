#############################################################
## RainOfFire_Pet_Activation.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	TestSphere
		Type	local
		At	root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			Scale		4.0 4.0 4.0
			EndScale	8.0 8.0 8.0
			Alpha		192
		End
	End

	Event
		EName	Hookup
		Type	local
		At	root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Geom	OverHeadDummy01
	End

	Event
		Type	local
		At	Hookup
		altpiv	1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\FireBlast\FireRain.part
		Part2	CustomizeablePowers\FireBlast\FireRains.part
	End
End

#############################################################

Condition
	On 	cycle
	Time 	6
	Chance	0.65

	Event
		Ename	Mushroom
		Type	Local
		At	Hookup
		altpiv	1
		ChildFx CustomizeablePowers\FireBlast\FireComet.fx
		Lifespan 75
	End
End

#############################################################

End