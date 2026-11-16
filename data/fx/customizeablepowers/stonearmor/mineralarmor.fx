#########################################################
##
##
FxInfo

#Flags	IsArmor


Flags    InheritAlpha

#####################################################################################
##Lens Flares
#####################################################


Condition
	On	Time
	Time 	0

	Event
		Type	Local
		At 	origin
		Sound Stone4 100 100 .96
	End
End

Condition
	On	Time
	Time 	8

	Event
		Type	Local
		At 	origin
		Sound Mineral_loop 100 100 .72
	End
End

Condition
	On	Time
	Time 	8

	Event
		Type	Local
		At 	origin
		Sound glow6_loop 100 100 .55
	End
End

#####################################################################################

Condition
	On 	cycle
	Time 	10
	Chance	.2

	Event
		EName	3
		Type	Local
		At	head
		part1	CustomizeablePowers\StoneArmor\FortitudeRaysSmall.part
		part2	CustomizeablePowers\StoneArmor\FortitudeRing.part
		part3	CustomizeablePowers\StoneArmor\FortitudeRing.part
		part4	CustomizeablePowers\StoneArmor\FortitudeDots.part

		lifespan	25
	End

End

####################################################################################################

Condition

	On	Time
	Time	30

	Event
		Ename	Drone
		Type	posit
		At	Root

		BHVR	CustomizeablePowers\StoneArmor\DamageDrone.bhvr


	End

	Event
		Ename	DroneSpin
		Type	local
		At	Drone

		BHVR	CustomizeablePowers\StoneArmor\DamageDroneSpin.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part	CustomizeablePowers\StoneArmor\MineralPsionicRingsFlat.part
		Part	CustomizeablePowers\StoneArmor\MineralPsionicRings.part
		geom	Tintable_Rock

	End


End


Condition

	On	Time
	Time	25

	Event
		Ename	Drone2
		Type	posit
		At	Root

		BHVR	CustomizeablePowers\StoneArmor\DamageDroneOpposite.bhvr


	End

	Event
		Ename	DroneSpin2
		Type	local
		At	Drone2

		BHVR	CustomizeablePowers\StoneArmor\DamageDroneSpinOpposite.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part	CustomizeablePowers\StoneArmor\MineralPsionicRingsFlat.part
		Part	CustomizeablePowers\StoneArmor\MineralPsionicRings.part
		geom	Tintable_rock

	End

End


Condition

	On	Time
	Time	45

	Event
		Ename	Drone3
		Type	posit
		At	Root

		BHVR	CustomizeablePowers\StoneArmor\DamageDroneOppositeSML.bhvr


	End

	Event
		Ename	DroneSpin3
		Type	local
		At	Drone3

		BHVR	CustomizeablePowers\StoneArmor\DamageDroneSpinOppositeSML.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part	CustomizeablePowers\StoneArmor\MineralPsionicRingsFlat.part
		Part	CustomizeablePowers\StoneArmor\MineralPsionicRings.part
		geom	Tintable_rock

	End


End

End
