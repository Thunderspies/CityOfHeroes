#########################################################
##	template

FxInfo

Input
	InpName Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Root
End

Input  
	InpName	Head
End

Input  
	InpName	WepL
End

Input  
	InpName	WepR
End



Lifespan 100

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On 	Time
	Time	12
	
	Event	
		Type 	Local
		At 	Origin
		Sound illusiontoss3 100 100 .75
	End
End


#########################################################
###################  Hand Casts  ########################
#########################################################


Condition
	On	Time
	Time	15

	Event	
		ENAME	LeftHand
		Type	Posit
		At	Head
		part	:IllusCastGlowRed01.part
		part	:IllusCastGlowRed02.part
		part	:IllusCastGlowRed03.part
		Lifespan 6
	
	End

	
End

Condition
	On	Time
	Time	25

	Event	
		ENAME	LeftHand
		Type	Posit
		At	Head
		#part	:IllusCastGlow02.part
		Lifespan 20
	
	End


End

Condition
	On	Time
	Time	60

	Event	
		ENAME	LeftHand
		Type	Posit
		At	Head
		#part	:IllusBlindGlow02.part
		Lifespan 10
	
	End


End


End		