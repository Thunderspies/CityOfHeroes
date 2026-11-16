#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo

########################################################

Condition
	On 	Cycle
	Time 	5
	Chance	.2

	Event
		Type	PositOnly
		At	Head
		part	:Fleas.part
		POther	Neck
		Lifespan	5
	End

End

#############################################################

End