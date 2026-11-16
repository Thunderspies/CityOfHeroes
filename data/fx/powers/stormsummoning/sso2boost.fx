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
	InpName	WepL
End

Input  
	InpName	WepR
End

Input  
	InpName	LlegL
End

Input  
	InpName	LlegR
End

Input  
	InpName	FootL
End

Input  
	InpName	FootR
End

Lifespan 200



#########################################################
#####################  Mist 1  #########################
#########################################################

Condition
	On 	Time
	Time	0

	Event	
		Type	Local
		At 	Origin
		Sound air1 100 100 .8
	End
End


Condition

	On 	Time	
	Time	0

	Event	
		ENAME	ThunderShake1
		Type	Posit
		At	Chest
		#part	:SSTClapHandLightning.part
		#part	:SSTClapHandFlashGlow.part
		#Bhvr	Behaviors/SSThunderClapShake.bhvr
		LifeSpan 10	
	End

End


End



	