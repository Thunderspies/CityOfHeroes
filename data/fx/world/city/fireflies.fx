#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo

########################################################

Condition
	On 	TimeOfDay
	DayStart 	19
	DayEnd		4

	Event
		EName	Fireflies
		Type	Start
		At	Origin
		part	:Fireflies.part
		Flags	OneAtATime
		Pmagnet	GnatAnchor
	#	Lifespan 1
	End
End

#############################################################

End