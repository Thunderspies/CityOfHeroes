#############################################################
## IceContinuing.fx
#############################################################

FxInfo
Flags IsArmor

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time 	0

	Event
		Type	Local
		At 	origin
		Sound Stone4 100 100 0.96
	End
End

Condition
	On	Time
	Time 	8

	Event
		Type	Local
		At 	origin
		Sound Mineral_loop 100 100 0.72
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 300
	End
End

Condition
	On	Time
	Time 	8

	Event
		Type	Local
		At 	origin
		Sound glow6_loop 100 100 .55
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 300
	End
End

#############################################################

Condition

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
		BHVR	V_COV\GenericPowerContinuingFx\ChestIce.bhvr
		Part	CustomizeablePowers\ColdDomination\Frost.part
		Part	CustomizeablePowers\ColdDomination\ColdArmMist.part
		Part	CustomizeablePowers\ColdDomination\SnowFall.part
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmor_HEAD
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
		BHVR	V_COV\GenericPowerContinuingFx\IceOpposite.bhvr
		Part	CustomizeablePowers\ColdDomination\Frost.part
		Part	CustomizeablePowers\ColdDomination\ColdArmMist.part
		Part	CustomizeablePowers\ColdDomination\SnowFall.part
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceArmor_HEAD
	End
End

Condition
	On	Time
	Time	5

	Event
		Ename	Drone3
		Type	posit
		At	Root
		BHVR	Powers\EarthControl\1DamageDroneSpinOppositeSML.bhvr
	End

	Event
		Ename	DroneSpin3
		Type	local
		At	Drone3
		BHVR	V_COV\GenericPowerContinuingFx\IceOppositeSML.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part	CustomizeablePowers\ColdDomination\Frost.part
		Part	CustomizeablePowers\ColdDomination\ColdArmMist.part
		Part	CustomizeablePowers\ColdDomination\SnowFall.part
		geom	Tintable_FX_IceArmor_HEAD
	End
End

#############################################################

End