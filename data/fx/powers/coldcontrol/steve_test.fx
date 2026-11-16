#########################################################
##	
##
FxInfo


Input  
	InpName	Root
End

Input  
	InpName	Chest
End

####################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	root
		geom	IceRootFlashFreeze
		
	End

End



Condition
	On	Death

	Event	
		Ename   IceShards
		Type	Local
		At	chest
		Part1	:Generic_hitflash.part
		Part2	:Generic_hitflash2.part
	

		lifespan	1
	End
End

End		