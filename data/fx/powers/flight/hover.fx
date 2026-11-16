#########################################################
##	chill touch hit
FxInfo


Input  
	InpName	FootR
End

Input  
	InpName	FootL
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

Input  
	InpName	Hips
End

##################################
Condition
	On	Time
	Time	0
	
	Event	
		Ename   targetchill
		Type	Posit
		At	FootR
		
		part3	:HoverSteam.part
		Part5	:HoverBodyMist.part
		Sound Hover_loop 100.0 100.0 .33

	End

	Event	
		Ename   targetchill
		Type	Posit
		At	FootL
		
		part3	:HoverSteam.part
		Part5	:HoverBodyMist.part

	End
		
	Event	
		Ename   targetchill
		Type	Posit
		At	WepL
		
		part3	:HoverSteam.part
		Part5	:HoverBodyMist.part

	End

	Event	
		Ename   targetchill
		Type	Posit
		At	WepR
		
		part3	:HoverSteam.part
		Part5	:HoverBodyMist.part

	End

End

End				