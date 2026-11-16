#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On	Time
	Time	3
	Event
		Type	Local
		At	WepR
		Part	:Hand_Buildup_Star_Small.part
		Part	:Hand_Buildup_Ring.part

	End


	Event
		Type	Local
		At	WepR
		Part	:Hand_Trail_Tiny.part
	#	Part	:Hand_Trail_Additive_Small.part
		Lifespan 4
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	WepL
		Part	:Hand_Buildup_Star_Small.part
		Part	:Hand_Buildup_Ring_Small.part

	End

	Event
		Type	Local
		At	WepL
		Part	:Hand_Trail_Tiny.part
	#	Part	:Hand_Trail_Additive_Small.part
		Lifespan 6
	End
End

#############################################################

End