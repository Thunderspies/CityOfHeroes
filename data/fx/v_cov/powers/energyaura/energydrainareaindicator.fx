#########################################################
##	template

FxInfo

LifeSpan	70

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound kinetic4 80 80 .84
	End
End


#########################################################
#################### AOE Indicator ########################
#########################################################



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
		LifeSpan	45
	End

	Event
		Type	local
		At	WepL

		Part	:EnergyGlow01.part
		Part	:EnergyGlow02.part
		Part	:EnergyGlowBlue01.part
		Part	:EnergyGlowBlue02.part
		LifeSpan	45
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		BHVR	:EnergyDrainRingSplat.bhvr
		Splat	electricring1copy.tga
		LifeSpan	16
	End


End

Condition
	On	Cycle
	Time	23


	Event
		Type	Local
		At	Root
		BHVR	:EnergyDrainRingSplat.bhvr
		Splat	electricring1copy.tga
		LifeSpan	16
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
			Alpha		255
			PyrRotate 	0 90 0
		End
		Geom	EarthGraspHit
		LifeSpan	30

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
		End
		Geom	EarthGraspHit
		LifeSpan	90
	End
End

#########################################################

End