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
		part	:SSMistRing01.part
		part	:SSMistRing02.part
		Bhvr	Behaviors/Spin2.bhvr
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
		part	:SSMistRing03.part
		Bhvr	Behaviors/Spin2.bhvr
		PMagnet Root
		LifeSpan 15
	End
End


End