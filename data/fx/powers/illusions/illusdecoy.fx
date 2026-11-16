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
	On	Time
	Time	6
	

	Event
		Type	Local
		At 	Origin
		Sound deceive1 80 80 .8
	End
End







#########################################################
###################  Hand Casts  ########################
#########################################################


Condition
	On	Time
	Time	6

	Event	
		ENAME	LeftHand
		Type	Posit
		At	WepL
		part	:IllusCastGlow01.part
		Lifespan 48
	
	End

End

Condition
	On	Time
	Time	25

	Event	
		ENAME	LeftHand
		Type	Posit
		At	WepL
		part	:IllusCastGlow02.part
		Lifespan 20
	
	End

End

Condition
	On	Time
	Time	45

	Event	
		ENAME	LeftHand
		Type	Posit
		At	WepL
		part	:IllusBlindGlow02.part
		Lifespan 10
	
	End

End


End		