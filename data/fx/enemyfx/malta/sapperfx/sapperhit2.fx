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

Lifespan 300

#########################################################
####################### AUDIO ###########################
#########################################################



Condition

	On 	Time	
	Time	0

	Event
		Type Local
		At origin
		Sound sapper2 50 50 .78
	End

End




Condition

	On 	Time	
	Time	40

	Event
		Type Local
		At origin
		#Sound	teletrans3 60 30 .33
	End

End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0
	
	Event	
		ENAME	NeckNode
		Type	Local
		At	Neck

	
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part
		Pmagnet NeckNode
		POther  NeckNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
	
	End

	Event	
		ENAME	ChestNode
		Type	Local
		At	Chest
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part
		Pmagnet ChestNode
		POther  ChestNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LarmRNode
		Type	Local
		At	LarmR
	
	End

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part
		Pmagnet LarmRNode
		POther  LarmRNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part
		Pmagnet LarmRnode
		POther  LarmRNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LarmLNode
		Type	Local
		At	LarmL
	
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part
		Pmagnet LarmLNode
		POther  LarmLNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part
		Pmagnet LarmLNode
		POther  LarmLNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LleglNode
		Type	Local
		At	Llegl
	
	End

	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part
		Pmagnet LleglNode
		POther  LleglNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part
		Pmagnet LleglNode
		POther  LleglNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LlegRNode
		Type	Local
		At	LlegR
	
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part
		Pmagnet LlegRNode
		POther  LlegRNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part
		Pmagnet LlegRNode
		POther  LlegRNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


End


End		