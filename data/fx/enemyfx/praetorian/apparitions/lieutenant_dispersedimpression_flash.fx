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
		Sound 	Apparitions_EnergyPulse_01 100 100 .4
		Sound 	Apparitions_EnergyPulse_02 100 100 .4
		Sound 	Apparitions_EnergyPulse_03 100 100 .4
		Sound 	Apparitions_EnergyPulse_04 100 100 .4
		Sound 	Apparitions_EnergyPulse_05 100 100 .4
		Sound 	Apparitions_EnergyPulse_06 100 100 .4
		Sound 	Apparitions_EnergyPulse_07 100 100 .4
		Sound 	Apparitions_EnergyPulse_08 100 100 .4
		Sound 	Apparitions_EnergyPulse_09 100 100 .4
		Sound 	Apparitions_EnergyPulse_10 100 100 .4
	End

	Event
		Type	Local
		At	Head
		part	:Body_Flash_Big_Lieu.part
		Part	:Body_Flash_Afterglow.part
		POther	Neck
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	Neck
		part	:Body_Flash_Lieu.part
		POther	Chest
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	Chest
		Part	:Fog_Coastal_Strip_Flash_Lieu.part
		Part	:Fog_Coastal_Strip_Flash_Highlight_Lieu.part
		part	:Body_Flash_Big.part
		POther	Hips
		Lifespan 	5
		LifespanJitter	3
	End


	Event
		Type	Local
		At	UArmL
		part	:Body_Flash_Lieu.part
		Part	:Body_Flash_Afterglow.part

		POther	LArmL
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	UArmR
		part	:Body_Flash_Lieu.part
		Part	:Body_Flash_Afterglow.part

		POther	LArmR
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	LArmL
		part	:Body_Flash_Lieu.part
		Part	:Body_Flash_Afterglow.part

		POther	WepL
		Lifespan 	3
		LifespanJitter	3
	End

	Event
		Type	Local
		At	LArmR
		part	:Body_Flash_Lieu.part
		Part	:Body_Flash_Afterglow.part

		POther	WepR
		Lifespan 	3
		LifespanJitter	3
	End

	Event
		Type	Local
		At	Hips
		part	:Body_Flash_Lieu.part
		Part	:Body_Flash_Afterglow.part

		POther	ULegL
	End

	Event
		Type	Local
		At	Hips
		part	:Body_Flash_Lieu.part
		Part	:Body_Flash_Afterglow.part

		POther	ULegR
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	ULegL
		part	:Body_Flash_Big_Lieu.part
		Part	:Body_Flash_Afterglow.part

		POther	LLegL
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	ULegR
		part	:Body_Flash_Big_Lieu.part
		Part	:Body_Flash_Afterglow.part

		POther	LLegR
		Lifespan 	5
		LifespanJitter	3
	End


	Event
		Type	Local
		At	FootL
		part	:Body_Flash_Big_Lieu.part
		Part	:Body_Flash_Afterglow.part

		POther	LLegL
		Lifespan 	3
		LifespanJitter	3
	End

	Event
		Type	Local
		At	FootR
		part	:Body_Flash_Big_Lieu.part
		Part	:Body_Flash_Afterglow.part

		POther	LLegR
		Lifespan 	3
		LifespanJitter	3
	End
End

#############################################################

End