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



Lifespan 90

#########################################################
#####################    Cast    ########################
#########################################################


Condition

	On 	Time	
	Time	0

	Event
		Type Local
		At origin
		Sound teletrans4 80 80 .9
	End

End



Condition
	On	Time
	Time	0
		
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:TSHeadGlow02.part
		Pmagnet HandR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 55
		
	End

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:TSHeadGlow02.part
		Pmagnet HandL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 55
		
	End
	

End

Condition
	On	Time
	Time	20
		
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		#part	:THandGlow1.part
		Pmagnet HandL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 55
		
	End
	

End

End

