#############################################################
## PlasmaContinuing.fx
#############################################################

FxInfo

Flags IsArmor

#############################################################

Condition
	On	Time
	Time 	8

	Event
		Type	Local
		At 	origin
		Sound Mineral_loop 70 70 .72
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
End

Condition
	On	Time
	Time 	8

	Event
		Type	Local
		At 	origin
		Sound plasmafire_loop 70 70 .55
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Drone
		Type	posit
		At	Root
		BHVR	Powers\EarthControl\DamageDrone.bhvr
	End

	Event
		Ename	DroneSpin
		Type	local
		At	Drone
		BHVR	Powers\EarthControl\ChestDamageDrone.bhvr
		Part	:PlasmaHandz.part
		Part	:PlasmaHandz2.part
		geom	rock
	End
End

Condition
	On	Time
	Time	2

	Event
		Ename	Drone2
		Type	posit
		At	Root
		BHVR	Powers\EarthControl\DamageDroneOpposite2.bhvr
	End

	Event
		Ename	DroneSpin2
		Type	local
		At	Drone2
		BHVR	Powers\EarthControl\ChestDamageDroneOpposite.bhvr
		Part	:PlasmaHandz.part
		Part	:PlasmaHandz2.part
		geom	rock
	End
End

Condition
	On	Time
	Time	5

	Event
		Ename	Drone3
		Type	posit
		At	Root
		BHVR	Powers\EarthControl\DamageDroneOppositeSML.bhvr
	End

	Event
		Ename	DroneSpin3
		Type	local
		At	Drone3
		BHVR	Powers\EarthControl\ChestDamageDroneOppositeSML.bhvr
		Part	:PlasmaHandz.part
		Part	:PlasmaHandz2.part
		geom	rock
	End
End

#############################################################

End