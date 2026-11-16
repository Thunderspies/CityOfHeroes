#########################################################
##	tech_doorgates
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

######################################################3
##Core
#############################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Origin

		Part	:Energy2ndLightHits.part
		
	End

End


End			