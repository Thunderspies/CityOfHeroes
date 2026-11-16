#############################################################
## RainOfFire.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

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
		part1	:FireRain.part
		Part2	:FireRains.part
	End

	Event
		EName	LastRainFalling
		Type	local
		At	Hookup
		altpiv	1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	:FireRain.part
		Part2	:FireRains.part
	End
End

#############################################################

Condition
	On 	Cycle
	Time 	6
	Chance	0.65

	Event
		Ename	Mushroom
		Type	Local
		At	Hookup
		altpiv	1
		ChildFx V_COV\PhysicsEnabled\FireComet.fx
		Lifespan	75
	End
End

#############################################################

End