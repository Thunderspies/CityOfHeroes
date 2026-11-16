#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo

########################################################

Condition
	On 		Triggerbits
	Triggerbits 	COMBAT
	DoMany		1

	Event
		EName	FlyAnchor
		Type	Local
		At	Chest
		While	COMBAT
		Flags	OneAtATime
		part	:Flies.part
		part	:Flies2.part
		part	:Beatles.part
		Pmagnet	Chest
	End
End

#############################################################

End