#############################################################
## Repulse.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound kinetic4 80 80 .84
	End
End

#############################################################

Condition
	On	cycle
	Time	10

	Event
		Type	Local
		At	Root
		BHVR	CustomizeablePowers\EnergyAura\RepulseScaleRotated.bhvr
		BhvrOverride
			TintGeom	1
			Alpha		100
		End
		GEOM	Tintable_Dark_RepulseWave
		LifeSpan 12
	End

	Event
		Type	Local
		At	Root
		BHVR	CustomizeablePowers\EnergyAura\RepulseScaleRotatedOpposite.bhvr
		BhvrOverride
			TintGeom	1
			Alpha		100

		End
		GEOM	Tintable_Dark_RepulseWaveInvert
		LifeSpan 12
	End
End

Condition
	On	cycle
	Time	10

	Event
		Type	Local
		At	Root
		BHVR	CustomizeablePowers\EnergyAura\RepulseScale.bhvr
		BhvrOverride
			TintGeom	1
			Alpha		100

		End
		GEOM	Tintable_Dark_RepulseWave
		LifeSpan 30
	End
End

#########################################################

End