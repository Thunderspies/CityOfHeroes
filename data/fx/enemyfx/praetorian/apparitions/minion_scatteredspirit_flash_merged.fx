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
		Sound 	Apparitions_EnergyPulse_01 75 75 .15
		Sound 	Apparitions_EnergyPulse_02 75 75 .15
		Sound 	Apparitions_EnergyPulse_03 75 75 .15
		Sound 	Apparitions_EnergyPulse_04 75 75 .15
		Sound 	Apparitions_EnergyPulse_05 75 75 .15
		Sound 	Apparitions_EnergyPulse_06 75 75 .15
		Sound 	Apparitions_EnergyPulse_07 75 75 .15
		Sound 	Apparitions_EnergyPulse_08 75 75 .15
		Sound 	Apparitions_EnergyPulse_09 75 75 .15
		Sound 	Apparitions_EnergyPulse_10 75 75 .15
	End


	Event
		Type	Local
		At	Head
		part	:Body_Flash_Small_Merged.part
		POther	Neck
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	Neck
		part	:Body_Flash_Big_Merged.part
		POther	Chest
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Fog_Coastal_Strip_Flash_Merged.part
		part	:Body_Flash_Big_Merged.part
		POther	Hips
		Lifespan 	4
		LifespanJitter	2
	End


	Event
		Type	Local
		At	UArmL
		part	:Body_Flash_Small_Merged.part
		POther	LArmL
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	UArmR
		part	:Body_Flash_Small_Merged.part
		POther	LArmR
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	LArmL
		part	:Body_Flash_Small_Merged.part
		POther	WepL
		Lifespan 	3
		LifespanJitter	3
	End

	Event
		Type	Local
		At	LArmR
		part	:Body_Flash_Small_Merged.part
		POther	WepR
		Lifespan 	3
		LifespanJitter	3
	End

	Event
		Type	Local
		At	Hips
		part	:Body_Flash_Small_Merged.part

		POther	ULegL
	End

	Event
		Type	Local
		At	Hips
		part	:Body_Flash_Small_Merged.part
		POther	ULegR
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	ULegL
		part	:Body_Flash_Merged.part
		POther	LLegL
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	ULegR
		part	:Body_Flash_Merged.part
		POther	LLegR
		Lifespan 	5
		LifespanJitter	3
	End


	Event
		Type	Local
		At	FootL
		part	:Body_Flash_Merged.part
		POther	LLegL
		Lifespan 	3
		LifespanJitter	3
	End

	Event
		Type	Local
		At	FootR
		part	:Body_Flash_Merged.part
		POther	LLegR
		Lifespan 	3
		LifespanJitter	3
	End
End

#############################################################

End