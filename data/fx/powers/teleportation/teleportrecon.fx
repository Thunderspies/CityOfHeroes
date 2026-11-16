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

Lifespan 300

#########################################################
####################### AUDIO ###########################
#########################################################

Condition

	On 	Time	
	Time	20

	Event
		Type Local
		At root
		Sound teletrans4 80 80 .85
	End

End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	20
		
	Event	
		ENAME	ChestGlow
		Type	Start
		At	Hips
		part1	:TSHeadGlow01.part
		Pmagnet Neck
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	HeadGlow
		Type	Start
		At	Hair
		part1	:TSHeadGlow01.part
		Pmagnet Chest
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	RightUarmGlow
		Type	Start
		At	SpadR
		part1	:TSHeadGlow01.part
		Pmagnet LarmR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	RightLarmGlow
		Type	Start
		At	WepR
		part1	:TSHeadGlow01.part
		Pmagnet LarmR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Start
		At	SpadL
		part1	:TSHeadGlow01.part
		Pmagnet LarmL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Start
		At	WepL
		part1	:TSHeadGlow01.part
		Pmagnet LarmL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftULegGlow
		Type	Start
		At	UlegL
		part1	:TSHeadGlow01.part
		Pmagnet Llegl
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Start
		At	FootL
		part1	:TSHeadGlow01.part
		Pmagnet Llegl
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Start
		At	UlegR
		part1	:TSHeadGlow01.part
		Pmagnet LlegR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Start
		At	FootR
		part1	:TSHeadGlow01.part
		Pmagnet LlegR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


End

#########################################################
#################### BODY GLOW 2 ########################
#########################################################

Condition
	On	Time
	Time	47
		
	Event	
		ENAME	ChestGlow
		Type	Start
		At	Hips
		part1	:TSHeadGlow02.part
		Pmagnet Neck
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
	
	End

	Event	
		ENAME	HeadGlow
		Type	Start
		At	Hair
		part1	:TSHeadGlow02.part
		Pmagnet Chest
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	RightUarmGlow
		Type	Start
		At	SpadR
		part1	:TSHeadGlow02.part
		Pmagnet LarmR
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	RightLarmGlow
		Type	Start
		At	WepR
		part1	:TSHeadGlow02.part
		Pmagnet LarmR
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Start
		At	SpadL
		part1	:TSHeadGlow02.part
		Pmagnet LarmL
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Start
		At	WepL
		part1	:TSHeadGlow02.part
		Pmagnet LarmL
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftULegGlow
		Type	Start
		At	UlegL
		part1	:TSHeadGlow02.part
		Pmagnet Llegl
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Start
		At	FootL
		part1	:TSHeadGlow02.part
		Pmagnet Llegl
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Start
		At	UlegR
		part1	:TSHeadGlow02.part
		Pmagnet LlegR
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Start
		At	FootR
		part1	:TSHeadGlow02.part
		Pmagnet LlegR
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
		
	End


End

#########################################################
################## WARP OUT FLASH #######################
#########################################################


Condition
	On	Time
	Time	33
		
	Event	
		ENAME	StarFlash0
		Type	Start
		At	Chest
		#part1	:TSWarpStar01.part
		Lifespan 18
	End

End

Condition
	On	Time
	Time	40
		
	Event	
		ENAME	GlowFlash
		Type	Start
		At	Chest
		#part1	:TSWarpGlow01.part
		Lifespan 10
	End

End

Condition
	On	Time
	Time	47
		
	Event	
		ENAME	GlowFlash
		Type	Start
		At	Chest
		#part1	:TSWarpGlow01.part
		Lifespan 10
	End

End

Condition
	On	Time
	Time	40
		
	Event	
		ENAME	FlashRays
		Type	Start
		At	Chest
		#part1	:TSWarpFlash01.part
		Pmagnet Chest
		Lifespan 10
	End

End

Condition
	On	Time
	Time	40
		
	Event	
		ENAME	StarFlash2
		Type	Start
		At	Chest
		#part1	:TSStar02.part
		Pmagnet Chest
		Lifespan 20
	End

End


End		