#########################################################
##	onfire
##
FxInfo  

Input  
	InpName	hips
End

Input  
	Inpname	chest
End

Input  
	Inpname	root
End

LifeSpan	5


Condition
	Time 	0

	Event
		EName	VaultHookup
		Type	local
		At	origin

		Part	:fireBallspark.part
		Sound stonehit2 10 10 .25

		
	End	


End			
