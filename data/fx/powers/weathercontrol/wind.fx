#########################################################
##	chill touch hit
FxInfo

Input  
	InpName Root
End

Input  
	InpName	hips
End

Input  
	InpName	Chest
End

##################################
Condition
	On	Time
	Time	40
	
	
	Event	
		Ename   targetchill
		Type	Local
		At	Root
		geom	AirFunnel02
		bhvr	behaviors/spin.bhvr
		Part1	:StreetTrash.part
		Part2	:StreetTrash2.part
		Part3	:WindClouds.part
	End
	
End


Condition
	On	Time
	Time	45

	Event	
		Ename   targetchill
		Type	Local
		At	Root
		PMAGNET	Root
		Part1	:O2BoostMistDownFlat.part
		Part2	:O2RingExpand.part
		
	End

End

End				