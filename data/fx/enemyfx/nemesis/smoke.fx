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
		EName 	NemesisSmoke1
		Type	local 
		At	Origin
		Part1	:Smoke.part
				
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

