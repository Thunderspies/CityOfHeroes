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
		At	Head
		part	:PossessedCitizen_Flash_Small.part
		POther	Neck
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	Neck
		part	:PossessedCitizen_Flash_Big.part
		POther	Chest
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	Chest
		part	:PossessedCitizen_Flash_Big.part
		POther	Hips
		Lifespan 	5
		LifespanJitter	3
	End


	Event
		Type	Local
		At	UArmL
		part	:PossessedCitizen_Flash_Small.part
		POther	LArmL
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	UArmR
		part	:PossessedCitizen_Flash_Small.part
		POther	LArmR
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	LArmL
		part	:PossessedCitizen_Flash_Small.part
		POther	WepL
		Lifespan 	3
		LifespanJitter	3
	End

	Event
		Type	Local
		At	LArmR
		part	:PossessedCitizen_Flash_Small.part
		POther	WepR
		Lifespan 	3
		LifespanJitter	3
	End

	Event
		Type	Local
		At	Hips
		part	:PossessedCitizen_Flash_Small.part

		POther	ULegL
	End

	Event
		Type	Local
		At	Hips
		part	:PossessedCitizen_Flash_Small.part
		POther	ULegR
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	ULegL
		part	:Body_Flash.part
		POther	LLegL
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	ULegR
		part	:Body_Flash.part
		POther	LLegR
		Lifespan 	5
		LifespanJitter	3
	End


	Event
		Type	Local
		At	FootL
		part	:Body_Flash.part
		POther	LLegL
		Lifespan 	3
		LifespanJitter	3
	End

	Event
		Type	Local
		At	FootR
		part	:Body_Flash.part
		POther	LLegR
		Lifespan 	3
		LifespanJitter	3
	End
End

#############################################################

End