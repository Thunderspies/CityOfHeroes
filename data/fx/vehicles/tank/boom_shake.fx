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
	InpName	Hips
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


Condition

	On 	Time	
	Time	0

	Event	
		ENAME	ThunderShake1
		Type	Local
		At	Origin
		Bhvr	Behaviors/SSThunderClapShake.bhvr
		
		
		
	End

End


End



	