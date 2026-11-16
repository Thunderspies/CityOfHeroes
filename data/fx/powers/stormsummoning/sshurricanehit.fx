#########################################################
##	template

FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Root
End

Input  
	InpName	Chest
End

Input  
	InpName	Hair
End



Lifespan 100



#########################################################
#####################  Mist 1  #########################
#########################################################

Condition

	On 	Time	
	Time	75

	Event	
		ENAME	CloudSmall
		Type	Posit
		At	Root
		#part	:SSMistRing01.part
		#part	:SSMistRing02.part
		Bhvr	Behaviors/Spin2.bhvr
		PMagnet Root
		LifeSpan 15
		
	End


End


End



	