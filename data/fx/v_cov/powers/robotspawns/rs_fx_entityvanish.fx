#########################################################

FxInfo

Input  
	InpName	Origin
End

Condition
	On	time
	time	0
	Event
		
		Type	Local
		At	Origin 
		Bhvr	behaviors/invisible.bhvr
		Flags	AdoptParentEntity
		#Sound	transformB 80 60 .86
	End
End

End			