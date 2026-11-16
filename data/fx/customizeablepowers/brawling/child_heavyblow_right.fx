#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On	Time
	Time	13
	Event
		Type	Local
		At	WepR
		Part	:Hand_Buildup_Star.part
		Part	:Hand_Buildup_Ring.part

	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	WepR
		Part	:Hand_Trail_Tiny.part
		Part	:Hand_Trail_Additive_Small.part
		Lifespan 10
	End
End

#############################################################

End