#########################################################
##	chill touch hit
FxInfo
Input  
	InpName Root
End

Input  
	InpName	head
End

LifeSpan	200

##################################
Condition
	On	Time
	Time	20
	
	Event	
		Ename   targetchill
		Type	Local
		At	Root
		PMAGNET	head
		Part3	:O2MistUpwards1.part
		Part4	:O2MistUpwards2.part
	End
		
End


Condition
	On	Time
	Time	0
	Event	
		Ename   targetchill2
		Type	Local
		At	Root
		PMAGNET	head
		Part1	:O2Mist.part
		Part2	:O2Mist2.part
		
	End
	
	Event	
		Ename   targetchill
		Type	Local
		At	Root
		geom	AirFunnel01
		bhvr	behaviors/spin.bhvr
		Part5	:O2Ring.part
		Part2	:O2BoostMistDownFlat.part
	End

End


End				