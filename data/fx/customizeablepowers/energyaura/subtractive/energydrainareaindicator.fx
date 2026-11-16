#############################################################
## EnergyDrainAreaIndicator.Fx
#############################################################

FxInfo

LifeSpan 70

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	2
		Type	Local
		At 	Root
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Sound kinetic4 80 80 .84
	End
End

Condition
	On	Time
	Time	25

	Event
		Type	local
		At	WepR
		Part	:EnergyGlow01.part
		Part	:EnergyGlow02.part
		Part	:EnergyGlowBlue01.part
		Part	:EnergyGlowBlue02.part
		LifeSpan 45
	End

	Event
		Type	local
		At	WepL
		Part	:EnergyGlow01.part
		Part	:EnergyGlow02.part
		Part	:EnergyGlowBlue01.part
		Part	:EnergyGlowBlue02.part
		LifeSpan 45
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	2
		BHVR	CustomizeablePowers\EnergyAura\EnergyDrainRingSplat_Sub.bhvr
		BhvrOverride
			Alpha		75
			PrimaryTint	75
			SecondaryTint	25
		End
		Splat	electricring1copy.tga
		LifeSpan 16
	End
End

Condition
	On	Cycle
	Time	23

	Event
		Type	Local
		At	2
		BHVR	CustomizeablePowers\EnergyAura\EnergyDrainRingSplat_Sub.bhvr
		BhvrOverride
			Alpha		75
			PrimaryTint	75
			SecondaryTint	25
		End
		Splat	electricring1copy.tga
		LifeSpan 16
	End
End

Condition
	On	Cycle
	Time	3

	Event
		Type	Start
		At	Root
		bhvr	V_COV\Powers\EnergyAura\EnergyDrainScale.bhvr
		BhvrOverride
			PyrRotate 	0 90 0
			Alpha		150
			TintGeom	1
		End
		Geom	Tintable_Dark_EarthGraspHit
		LifeSpan	1

	End
End

Condition
	On	Time
	Time	2

	Event
		Type	Start
		At	Root
		bhvr	V_COV\Powers\EnergyAura\EnergyDrainScale.bhvr
		BhvrOverride
			PyrRotate 	0 90 0
			Alpha		150

			TintGeom	1
		End
		Geom	Tintable_Dark_EarthGraspHit
		LifeSpan	90

	End

End

#############################################################

End