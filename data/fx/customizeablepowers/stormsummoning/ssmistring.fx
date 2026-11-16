#########################################################
## Storm Summoning - Steamy Mist Ring
#########################################################

FxInfo

Lifespan 100

#########################################################

Condition

	On 	Time	
	Time	75

	Event	
		ENAME	CloudSmall
		Type	Posit
		At	Root
		part	CustomizeablePowers\StormSummoning\SSMistRing01.part
		part	CustomizeablePowers\StormSummoning\SSMistRing02.part
		Bhvr	CustomizeablePowers\StormSummoning\Spin2.bhvr
		PMagnet Root
		LifeSpan 15
	End
End

Condition

	On 	Time	
	Time	65

	Event	
		ENAME	CloudSmall
		Type	Posit
		At	Root
		part	CustomizeablePowers\StormSummoning\SSMistRing03.part
		Bhvr	CustomizeablePowers\StormSummoning\Spin2.bhvr
		PMagnet Root
		LifeSpan 15
	End
End


End