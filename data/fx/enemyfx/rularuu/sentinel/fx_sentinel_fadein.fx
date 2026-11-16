#########################################################

FxInfo

Input  
	InpName	Origin
End

LifeSpan	30

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin 
		Bhvr	behaviors/Sentinelfadein.bhvr
		#Flags	AdoptParentEntity
		LifeSpan	20
	End
End


End			