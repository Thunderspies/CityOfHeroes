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
	InpName	Chest
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



#Lifespan 200



#########################################################
#####################  Mist 1  #########################
#########################################################


Condition

	On 	Time	
	Time 	25
	
	Event	
		Type	Local
		At 	Origin
		Sound fog_loop 100 100 .7
	End

End



Condition

	On 	Time	
	Time	55

	Event	
		ENAME	BodyMist
		Type	Local
		At	Chest
		part	:SSBodyFog01.part
		#Bhvr	Behaviors/Spin2.bhvr
		#LifeSpan 70
		
			
	End

	Event	
		ENAME	BodyMist
		Type	Local
		At	WepL
		part	:SSBodyFog01.part
		#Bhvr	Behaviors/Spin2.bhvr
		#LifeSpan 70
		
	End

	Event	
		ENAME	BodyMist
		Type	Local
		At	WepR
		part	:SSBodyFog01.part
		#Bhvr	Behaviors/Spin2.bhvr
		#LifeSpan 70
		
	End

	Event	
		ENAME	BodyMist
		Type	Local
		At	LlegL
		part	:SSBodyFog01.part
		#Bhvr	Behaviors/Spin2.bhvr
		#LifeSpan 70
		
	End

	Event	
		ENAME	BodyMist
		Type	Local
		At	LlegR
		part	:SSBodyFog01.part
		#Bhvr	Behaviors/Spin2.bhvr
		#LifeSpan 70
		
	End

	Event	
		ENAME	BodyMist
		Type	Local
		At	FootL
		part	:SSBodyFog01.part
		#Bhvr	Behaviors/Spin2.bhvr
		#LifeSpan 70
		
	End

	Event	
		ENAME	BodyMist
		Type	Local
		At	FootR
		part	:SSBodyFog01.part
		#Bhvr	Behaviors/Spin2.bhvr
		#LifeSpan 70
		
	End


End


End



	