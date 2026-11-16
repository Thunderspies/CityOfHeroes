#########################################################
##	template

FxInfo

Input  
	InpName	Root
End

Input  
	InpName	Head
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



#########################################################
####################### AUDIO ###########################
#########################################################



#########################################################
###### HEAD GLOWS #####################################
#########################################################


Condition
	On 	Time  
	Time 	0
	
	Event
		EName 	Infernal_Head_Node
		Type	Local 
		At	Hair
		GEOM	InfernalEyes
	
	End
	Event
		EName 	InfernalEyes
		Type	Local 
		At	Infernal_Head_Node
		AltPiv	1
		Part	:InfernalEyeGlow.part
		Part	:InfernalEyeGlow1.part
		Part	:InfernalEyeGlow2.part

	End
	Event
		EName 	InfernalEyes
		Type	Local 
		At	Infernal_Head_Node
		AltPiv	2
		Part	:InfernalEyeGlow.part
		Part	:InfernalEyeGlow1.part
		Part	:InfernalEyeGlow2.part	

	End


End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0
	

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		#part1	:InfernalBodyGlowLines01.part
		part1	:InfernalBodyGlows01.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		#part1	:InfernalBodyGlowLines01.part
		part	:InfernalBodyStreaks01.part
		part	:InfernalBodyStreaks02.part
		part	:InfernalBodyGlows01.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		#part1	:InfernalBodyGlowLines01.part
		part1	:InfernalBodyGlows01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:InfernalBodyStreaks01.part
		part	:InfernalBodyStreaks02.part
		#part1	:InfernalBodyGlowLines01.part
		part1	:InfernalBodyGlows01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		#part1	:InfernalBodyGlowLines01.part
		part1	:InfernalBodyGlows01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:InfernalBodyStreaks01.part
		part	:InfernalBodyStreaks02.part
		#part1	:InfernalBodyGlowLines01.part
		part1	:InfernalBodyGlows01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		#part1	:InfernalBodyGlowLines01.part
		part1	:InfernalBodyGlows01.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:InfernalBodyStreaks01.part
		part	:InfernalBodyStreaks02.part
		#part1	:InfernalBodyGlowLines01.part
		part1	:InfernalBodyGlows01.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		#part1	:InfernalBodyGlowLines01.part
		part1	:InfernalBodyGlows01.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:InfernalBodyStreaks01.part
		part	:InfernalBodyStreaks02.part
		#part1	:InfernalBodyGlowLines01.part
		part1	:InfernalBodyGlows01.part
		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 3
		
	End


End


#########################################################
###### KILL FX  #####################################
#########################################################

Condition
	On		Triggerbits
	Triggerbits	Death

	Event		
		Ename	All
		Type	Destroy

	End

End

End	