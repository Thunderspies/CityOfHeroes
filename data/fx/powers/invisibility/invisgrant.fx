#########################################################
##	template

FxInfo

Input  
	InpName	Root
End

Input  
	InpName	Hair
End

Input  
	InpName	Eyes
End

Input  
	InpName	Neck
End

Input  
	InpName	Chest
End

Input  
	InpName	SpadR
End

Input  
	InpName	SpadL
End

Input  
	InpName	UarmR
End

Input  
	InpName	UarmL
End

Input  
	InpName	LarmR
End

Input  
	InpName	LarmL
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

Input  
	InpName	UlegR
End

Input  
	InpName	UlegL
End

Input  
	InpName	LlegR
End

Input  
	InpName	LlegL
End

Input  
	InpName	FootR
End

Input  
	InpName	FootL
End

Input  
	InpName	ToeL
End

Input  
	InpName	ToeR
End

Input
	InpName Origin
End

Lifespan 100


#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound Invisible1 80 80 .5
	End
End




#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	10
	
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:InvisCastGlow01.part
		part	:InvisBodyGlow02.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 40
		
	End


End


End		