#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo

########################################################

Condition
#	On	Time
#	Time	0

	On 	TimeOfDay
	DayStart 	19
	DayEnd		6

	Event
		EName	FirefliesSmall
		Type	Start
		At	Origin
		part	:Fireflies_Small.part
		Flags	OneAtATime

		Pmagnet	GnatAnchor
	End
End

#############################################################

End