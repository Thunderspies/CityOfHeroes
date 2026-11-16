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



Lifespan 150

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time 	8
	
	Event
		Type	Local
		At 	Origin
		Sound deceive1 100 100 .7
	End
End


#########################################################
###################  Hand Casts  ########################
#########################################################


Condition
	On	Time
	Time	12

	Event	
		ENAME	LeftHand
		Type	Posit
		At	WepL
		part	CustomizeablePowers\IllusionControl\IllusCastGlow01.part
		Lifespan 80
	
	End

	Event	
		ENAME	RightHand
		Type	Posit
		At	WepR
		part	CustomizeablePowers\IllusionControl\IllusCastGlow01.part
		Lifespan 80
	
	End

End

Condition
	On	Time
	Time	25

	Event	
		ENAME	LeftHand
		Type	Posit
		At	WepL
		part	CustomizeablePowers\IllusionControl\IllusCastGlow02.part
		Lifespan 21
	
	End

	Event	
		ENAME	RightHand
		Type	Posit
		At	WepR
		part	CustomizeablePowers\IllusionControl\IllusCastGlow02.part
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
		part	CustomizeablePowers\IllusionControl\IllusCastGlow02.part
		Lifespan 20
	
	End

	Event	
		ENAME	RightHand
		Type	Posit
		At	WepR
		part	CustomizeablePowers\IllusionControl\IllusCastGlow02.part
		Lifespan 21
	
	End

End


End		