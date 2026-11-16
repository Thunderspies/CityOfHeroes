#############################################################
## PlasmaShield_Continuing.FX
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
		Sound Mineral_loop 70 70 0.72
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
		Sound plasmafire_loop 70 70 0.55
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
End

#############################################################

Condition

	Event
		Ename	Drone
		Type	posit
		At	Root
		BHVR	CustomizeablePowers\ThermalRadiation\DamageDrone.bhvr
	End

	Event
		Ename	DroneSpin
		Type	local
		At	Drone
		BHVR	CustomizeablePowers\ThermalRadiation\ChestDamageDrone.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part	CustomizeablePowers\ThermalRadiation\PlasmaHandz.part
		Part	CustomizeablePowers\ThermalRadiation\PlasmaHandz2.part
		geom	Tintable_Rock
	End

End


Condition
	On	Time
	Time	2

	Event
		Ename	Drone2
		Type	posit
		At	Root
		BHVR	CustomizeablePowers\ThermalRadiation\DamageDroneOpposite2.bhvr

	End

	Event
		Ename	DroneSpin2
		Type	local
		At	Drone2
		BHVR	CustomizeablePowers\ThermalRadiation\ChestDamageDroneOpposite.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part	CustomizeablePowers\ThermalRadiation\PlasmaHandz.part
		Part	CustomizeablePowers\ThermalRadiation\PlasmaHandz2.part
		geom	Tintable_Rock
	End
End


Condition
	On	Time
	Time	5

	Event
		Ename	Drone3
		Type	posit
		At	Root
		BHVR	CustomizeablePowers\ThermalRadiation\DamageDroneOppositeSML.bhvr

	End

	Event
		Ename	DroneSpin3
		Type	local
		At	Drone3
		BHVR	CustomizeablePowers\ThermalRadiation\ChestDamageDroneOppositeSML.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part	CustomizeablePowers\ThermalRadiation\PlasmaHandz.part
		Part	CustomizeablePowers\ThermalRadiation\PlasmaHandz2.part
		geom	Tintable_Rock
	End

End
End
