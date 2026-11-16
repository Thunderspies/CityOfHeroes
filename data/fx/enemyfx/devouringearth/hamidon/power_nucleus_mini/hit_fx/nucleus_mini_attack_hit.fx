#########################################################
##	template

FxInfo

Input  
	InpName	Hips
End

Input  
	InpName	Waist
End

Input  
	InpName	FootL
End

Input  
	InpName	FootR
End

Input  
	InpName	ULEGL
End

Input  
	InpName	ULEGR
End

Input  
	InpName	LLEGL
End

Input  
	InpName	LLEGR
End

Input  
	InpName	wepR
End

Input  
	InpName	wepL
End

Input  
	InpName	HIPS
End

Input  
	InpName	CHEST
End

Input  
	InpName	Head
End

Input  
	InpName	UARMR
End

Input  
	InpName	UARML
End

Input  
	InpName	LARML
End

Input  
	InpName	LARMR
End

Input  
	InpName	Root
End 

Flags    InheritAlpha

#########################################################




Condition
	On 	Time
	Time 	0	
	
	Event
		EName 	Hamidon
		Type	local
		At	Waist
		Part	:Nucleus_Mini_Burst.part
		sound	hamidonMiniHit 50 40 .66
		Lifespan	40
	End
End

Condition
	On 	Time
	Time 	0	
	
	Event
		EName 	Hamidon
		Type	local
		At	Waist
		Part	:Nucleus_Mini_Sparks.part
		Lifespan	3
	End
End

