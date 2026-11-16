#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo

########################################################

Condition
	On 	Cycle
	Time 	5
	Chance	.35
	Random	1

	Event
		Type	PositOnly
		At	Head
		part	:Fleas.part
		POther	Neck
		Lifespan	5
	End

	Event
		Type	PositOnly
		At	UArmL
		part	:Fleas.part
		POther	LArmL

		Lifespan	5

	End

	Event
		Type	PositOnly
		At	UArmR
		part	:Fleas.part
		POther	LArmR
		Lifespan	5
	End

	Event
		Type	PositOnly
		At	LArmL
		part	:Fleas.part
		POther	WepL
		Lifespan	5

	End

	Event
		Type	PositOnly
		At	LArmR
		part	:Fleas.part
		POther	WepR
		Lifespan	5

	End

	Event
		Type	PositOnly
		At	LLegL
		part	:Fleas.part
		POther	FootL
		Lifespan	5

	End

	Event
		Type	PositOnly
		At	LLegR
		part	:Fleas.part
		POther	FootR
		Lifespan	5

	End

	Event
		Type	PositOnly
		At	FootL
		part	:Fleas.part
		POther	LLegL
		Lifespan	5

	End

	Event
		Type	PositOnly
		At	FootR
		part	:Fleas.part
		POther	LLegR
		Lifespan	5

	End
End

#############################################################

End