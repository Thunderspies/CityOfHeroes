#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo

########################################################

Condition
	On 	Cycle
	Time 	5
	Chance	.15
	Random	1

	Event
		Type	PositOnly
		At	UArmL
		part	:Fleas.part
		POther	LArmL

		Lifespan	5

	End

	Event
		Type	PositOnly
		At	LArmL
		part	:Fleas.part
		POther	WepL
		Lifespan	5

	End
End

#############################################################

End