#############################################################
## EnergyDrain.Fx
#############################################################

FxInfo
LifeSpan	200

#############################################################

Input
	Inpname	Hips
End

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
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0


End

Condition
	On	Time
	Time	0

	Event
		EName	prime
		Type	Start
		At	Chest

		Part	:EnergyGlow01.part
		Part	:EnergyGlow02.part
		Part	:EnergyGlowBlue01.part
		Part	:EnergyGlowBlue02.part

		bhvr	CustomizeablePowers\EnergyAura\MedianProjectile3.bhvr

		Magnet	T_WepR
		LookAt  T_WepR


	End

	Event
		EName	prime1
		Type	Start
		At	Chest

		Part	:EnergyGlow01.part
		Part	:EnergyGlow02.part
		Part	:EnergyGlowBlue01.part
		Part	:EnergyGlowBlue02.part

		bhvr	CustomizeablePowers\EnergyAura\MedianProjectile3.bhvr

		Magnet	T_WepL
		LookAt  T_WepL


	End

End

Condition
	On 	PrimeHit



	Event
		EName	Prime
		Type	Destroy

	End

End

Condition
	On 	Prime1Hit


	Event
		EName	Prime1
		Type	Destroy

	End


End

End