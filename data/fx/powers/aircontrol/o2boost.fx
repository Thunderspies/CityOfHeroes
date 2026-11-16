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

LifeSpan	200

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
		Part5	:O2Ring.part
	End
	
	Event	
		Ename   targetchill
		Type	Local
		At	Root
		PMAGNET	Root
		Part1	:O2BoostMistDown1.part
		Part2	:O2BoostMistDown2.part
	End
		
End

Condition
	On	Time
	Time	0

	Event	
		Ename   targetchill2
		Type	Local
		At	Chest
		Part1	:O2BoostMist.part
		Part2	:O2BoostMist2.part
		
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
		Part2	:O2BoostMistDownFlat.part
		
	End

End

End				