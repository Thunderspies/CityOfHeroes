#############################################################
## Steam_High.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		SoundNoRepeat 7
		Sound 	Apparitions_EnergyPulse_01 100 100 .6
		Sound 	Apparitions_EnergyPulse_02 100 100 .6
		Sound 	Apparitions_EnergyPulse_03 100 100 .6
		Sound 	Apparitions_EnergyPulse_04 100 100 .6
		Sound 	Apparitions_EnergyPulse_05 100 100 .6
		Sound 	Apparitions_EnergyPulse_06 100 100 .6
		Sound 	Apparitions_EnergyPulse_07 100 100 .6
		Sound 	Apparitions_EnergyPulse_08 100 100 .6
		Sound 	Apparitions_EnergyPulse_09 100 100 .6
		Sound 	Apparitions_EnergyPulse_10 100 100 .6
	End

	Event
		Type	Local
		At	Root
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 6.0 0.0
			StartJitter	1.0 4.0 1.0
		End
		Part	:Flash_Tall_Highlight.part
		POther	Hips
		Lifespan 	5
		LifespanJitter	3
	End
End

Condition
	On	Time
	Time	0

	Chance	0.4

	Random 1

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 6.0 0.0
			StartJitter	1.0 1.0 1.0
		End
		Part	:Face.part
		part	:Flash_Central.part
		Part	:Flash_linger.part

		Lifespan 	6
		LifespanJitter	3
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 7.0 0.0
			StartJitter	1.0 2.0 1.0
		End
		Part	:Face2.part
		part	:Flash_Central.part
		Part	:Flash_linger.part
		Lifespan 	6
		LifespanJitter	3
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 7.0 0.0
			StartJitter	1.0 2.0 1.0
		End
		Part	:Face3.part
		part	:Flash_Central.part
		Part	:Flash_linger.part

		Lifespan 	6
		LifespanJitter	3
	End
End
#############################################################

End