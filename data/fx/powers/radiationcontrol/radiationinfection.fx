#########################################################
## Radiation Infection
#########################################################

FxInfo
Lifespan 60
ClampMaxScale 1 1 1

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ringsdfgf
		Type	posit
		At	T_root
		Part1	:RadiationRingsFlat2.part
	End

	Event
		EName 	OrbStreak
		Type	posit
		At	T_root
		geom	centerdummy
		bhvr	behaviors/Rune_spinFasterx.bhvr
	End

	Event
		EName 	Orb
		Type	posit
		At	OrbStreak
		altpiv  1
		part2	:EnervatingMist2.part
		part3	:EnervatingMistGlows.part
	End
End

#############################################################

Condition

	Event
		Type	posit
		At	T_Head
		part	:RadiationInfectionGlow1.part
		part	:RadiationInfectionGlow2.part
		Pmagnet T_Head
	End

	Event
		Type	posit
		At	T_Chest
		part	:RadiationInfectionGlow1.part
		part	:RadiationInfectionGlow2.part
		Pmagnet T_Hips
		POther  T_Hips
	End

	Event
		Type	posit
		At	T_Hips
		part	:RadiationInfectionGlow1.part
		part	:RadiationInfectionGlow2.part
		Pmagnet T_UArmL
		POther  T_UArmL
	End

	Event
		Type	posit
		At	T_Hips
		part	:RadiationInfectionGlow1.part
		part	:RadiationInfectionGlow2.part
		Pmagnet T_UarmR
		POther  T_UarmR
	End

	Event
		Type	posit
		At	T_UarmR
		part	:RadiationInfectionGlow1.part
		part	:RadiationInfectionGlow2.part
		Pmagnet T_LarmR
		POther  T_LarmR
	End

	Event
		Type	posit
		At	T_WepR
		part	:RadiationInfectionGlow1.part
		part	:RadiationInfectionGlow2.part
		Pmagnet T_LarmR
		POther  T_LarmR
	End

	Event
		Type	posit
		At	T_UarmL
		part	:RadiationInfectionGlow1.part
		part	:RadiationInfectionGlow2.part
		Pmagnet T_LarmL
		POther  T_LarmL
	End

	Event
		Type	posit
		At	T_WepL
		part	:RadiationInfectionGlow1.part
		part	:RadiationInfectionGlow2.part
		Pmagnet T_LarmL
		POther  T_LarmL
	End

	Event
		Type	posit
		At	T_UlegL
		part	:RadiationInfectionGlow1.part
		part	:RadiationInfectionGlow2.part
		Pmagnet T_LlegL
		POther  T_LlegL
	End

	Event
		Type	posit
		At	T_UlegR
		part	:RadiationInfectionGlow1.part
		part	:RadiationInfectionGlow2.part
		Pmagnet T_LlegR
		POther  T_LlegR
	End

	Event
		Type	posit
		At	T_LlegR
		part	:RadiationInfectionGlow1.part
		part	:RadiationInfectionGlow2.part
		Pmagnet T_FootR
		POther  T_FootR
	End

	Event
		Type	posit
		At	T_LlegL
		part	:RadiationInfectionGlow1.part
		part	:RadiationInfectionGlow2.part
		Pmagnet T_FootL
		POther  T_FootL
	End
End

#############################################################

End