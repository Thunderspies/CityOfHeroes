#########################################################
##	Inferno FX
##
FxInfo
	
#lifespan	900

##########################################################################################################
##Final Blast
######################################################################################3

Condition
	On 	Time
	Time 	0

	Event
		EName 	Primex
		Type	Local 
		At	Root
		Bhvr	:Offset.bhvr
		Part	:AshBitsBlask.part		
		Part	:WhiteDeathSmoke.part	
		Part	:WhiteDeathSteam.part
		#LifeSpan	900
	End

End


End
