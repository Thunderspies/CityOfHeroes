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
		part1	:FreezingStormRain.part
		Part2	:FreezingStormRains.part
		Part3	:FreezingIceSteam.part
		Sound icicle_loop 80 80 .55
	End
End

#############################################################

End