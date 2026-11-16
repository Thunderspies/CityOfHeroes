#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Flags    InheritAlpha
########################################################

Condition
	On 	Time
	Time 	0

	
	
	Event
		EName 	NemesisSteam1
		Type	local 
		At	Origin
		Part	:Steam.part
		Part	:Steam2.part
				
	End
	
	
End

Condition
	On 	Death
		
	Event
		
		Type	local 
		At	Target
		
		LifeSpan	20
				
	End

End

End
##################################

