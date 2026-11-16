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
		#Sound	laser1_loop 60 50 .33
	End
End


End			