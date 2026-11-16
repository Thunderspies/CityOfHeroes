#########################################################
##	TechSecurity
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	start
		At	Origin
		Part1	:TechSecurityLasers.part
		
	End
End


End			