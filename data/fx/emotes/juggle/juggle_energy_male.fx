#############################################################
## HEADER
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	JuggleRig
		Type	Local
		At	Mystic
		Anim	FX_JuggleBalls_Male
	End
End

## BALLS ###########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Ball1
		Type	Local
		At	JuggleRig
		AnimPiv	Neck
		BhvrOverride
			PyrRotateJitter	180 0.0 180
		End
	#	Part1	POWERS\EnergyBlast\CoreEnergySmall.part
		Part2	POWERS\EnergyBlast\CometBubblesOrbiting.part
		Part3	POWERS\ElectricityControl\GlowRingSmall3.part
		Part4	POWERS\EnergyBlast\JuggleBallGlow.part
		Part5	POWERS\EnergyBlast\CometBubblesOrbitingWhite.part

		Pmagnet	Ball1
		Geom	GEO_GenericBall_01
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName 	Ball2
		Type	Local
		At	JuggleRig
		AnimPiv	Waist
		BhvrOverride
			StartColor	0 255 0
		End
	#	Part1	POWERS\EnergyBlast\CoreEnergySmall.part
		Part2	POWERS\EnergyBlast\CometBubblesOrbiting.part
		Part3	POWERS\ElectricityControl\GlowRingSmall3.part
		Part4	POWERS\EnergyBlast\JuggleBallGlow.part
		Part5	POWERS\EnergyBlast\CometBubblesOrbitingWhite.part

		Geom	GEO_GenericBall_01
	End
End

Condition
	On 	Time
	Time 	35

	Event
		EName 	Ball3
		Type	Local
		At	JuggleRig
		AnimPiv	Chest
		BhvrOverride
			StartColor	0 0 255
		End
	#	Part1	POWERS\EnergyBlast\CoreEnergySmall.part
		Part2	POWERS\EnergyBlast\CometBubblesOrbiting.part
		Part3	POWERS\ElectricityControl\GlowRingSmall3.part
		Part4	POWERS\EnergyBlast\JuggleBallGlow.part
		Part5	POWERS\EnergyBlast\CometBubblesOrbitingWhite.part

		Geom	GEO_GenericBall_01
	End
End


#############################################################
