#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Chestnode
		Type	Local
		At	Origin
		Geom	ChestPivotsBuffDef
	End
End

Condition
	On	Time
	Time	0

	Event
		ENAME	Front
		Type	Local
		At	Chestnode
		AltPiv	3
		part	:DeBuffEndChevron01.part
		part	:DeBuffEndChevron03.part
		part	:DeBuffEndChevron02.part
		Lifespan 60
	End

	Event
		ENAME	Back
		Type	Local
		At	Chestnode
		AltPiv	4
		part	:DeBuffEndChevron01.part
		part	:DeBuffEndChevron03.part
		part	:DeBuffEndChevron02.part
		Lifespan 60
	End
End

#############################################################

End