#############################################################
## IceStorm.fx
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
		Sound icestorm 100 100 .88
	End

	Event
		Type	local
		At	Hookup
		altpiv	1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	POWERS\ColdControl\FreezingStormRain.part
		Part2	POWERS\ColdControl\FreezingStormRains.part
		Part3	POWERS\ColdControl\FreezingIceSteam.part
		Sound icicle_loop 80 80 .55
	End

#	Event
#		EName	LastRainFalling
#		Type	local
#		At	Hookup
#		altpiv	1
#		Bhvr	Behaviors\GenericParticleFade.bhvr
#		part1	POWERS\ColdControl\FreezingStormRain.part
#		Part2	POWERS\ColdControl\FreezingStormRains.part
#		Part3	POWERS\ColdControl\FreezingIceSteam.part
#		Lifespan 120
#	End
End

#############################################################

End