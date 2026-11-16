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
	InpName	WepL
End

Input  
	InpName	WepR
End



Lifespan 200

#########################################################
####################### AUDIO ###########################
#########################################################




#########################################################
#################### BODY GLOW 1 ########################
#########################################################





Condition
	On	Time
	Time	0

	Event	
		ENAME	LeftHand
		Type	Local
		At	WepL
		part	:IllusFlashFlash01.part
		#Lifespan 7
	
	End

	Event	
		ENAME	RightHand
		Type	Local
		At	WepR
		part	:IllusFlashFlash01.part
		#Lifespan 7
	
	End

End

End		