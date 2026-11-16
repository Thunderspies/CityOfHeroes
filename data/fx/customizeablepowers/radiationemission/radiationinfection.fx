#############################################################
## RadiationInfection.fx
#############################################################

FxInfo

Lifespan 60

ClampMaxScale 1 1 1

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	posit
		At	T_root
		Part1	CustomizeablePowers\RadiationEmission\RadiationRingsFlat2.part
	End

	Event
		EName 	OrbStreak
		Type	posit
		At	T_root
		geom	centerdummy
		bhvr	CustomizeablePowers\RadiationEmission\Rune_spinFasterx.bhvr
	End

	Event
		EName 	Orb
		Type	posit
		At	OrbStreak
		altpiv  1
		part2	CustomizeablePowers\RadiationEmission\EnervatingMist2.part
		part3	CustomizeablePowers\RadiationEmission\EnervatingMistGlows.part
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	posit
		At	T_Head
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow1.part
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow2.part
		Pmagnet T_Head
	End

	Event
		Type	posit
		At	T_Chest
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow1.part
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow2.part
		Pmagnet T_Hips
		POther  T_Hips
	End

	Event
		Type	posit
		At	T_Hips
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow1.part
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow2.part
		Pmagnet T_UArmL
		POther  T_UArmL
	End

	Event
		Type	posit
		At	T_Hips
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow1.part
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow2.part
		Pmagnet T_UarmR
		POther  T_UarmR
	End

	Event
		Type	posit
		At	T_UarmR
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow1.part
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow2.part
		Pmagnet T_LarmR
		POther  T_LarmR
	End

	Event
		Type	posit
		At	T_WepR
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow1.part
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow2.part
		Pmagnet T_LarmR
		POther  T_LarmR
	End

	Event
		Type	posit
		At	T_UarmL
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow1.part
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow2.part
		Pmagnet T_LarmL
		POther  T_LarmL
	End

	Event
		Type	posit
		At	T_WepL
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow1.part
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow2.part
		Pmagnet T_LarmL
		POther  T_LarmL
	End

	Event
		Type	posit
		At	T_UlegL
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow1.part
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow2.part
		Pmagnet T_LlegL
		POther  T_LlegL
	End

	Event
		Type	posit
		At	T_UlegR
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow1.part
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow2.part
		Pmagnet T_LlegR
		POther  T_LlegR
	End

	Event
		Type	posit
		At	T_LlegR
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow1.part
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow2.part
		Pmagnet T_FootR
		POther  T_FootR
	End

	Event
		Type	posit
		At	T_LlegL
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow1.part
		part	CustomizeablePowers\RadiationEmission\RadiationInfectionGlow2.part
		Pmagnet T_FootL
		POther  T_FootL
	End
End

#############################################################

End