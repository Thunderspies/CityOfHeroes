#########################################################
##	template

FxInfo

Input  
	InpName	Root
End

Input  
	InpName	HandR
End

Input  
	InpName	HandL
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End



Lifespan 240

#########################################################
#####################    Cast    ########################
#########################################################

Condition
	On	Time
	Time	5
		
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:TSHeadGlow02.part
		Pmagnet HandR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 0
		SOund	teletrans5 80 30 .88
		
	End

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:TSHeadGlow02.part
		Pmagnet HandL
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 0
		
	End
	

End

End

