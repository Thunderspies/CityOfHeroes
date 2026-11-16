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

Lifespan 350

#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition

	On	Time
	Time	20
	
	Event	
		ENAME	NeckNode
		Type	Start
		At	Neck
	End

	Event	
		ENAME	ChestGlow
		Type	Start
		At	Hips
		part1	:GCBodyGlow.part
		Pmagnet NeckNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
	
	End

	Event	
		ENAME	ChestNode
		Type	Start
		At	Chest
		LifeSpan 70
	End

	Event	
		ENAME	HeadGlow
		Type	Start
		At	Hair
		part1	:GCBodyGlow.part
		Pmagnet ChestNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
		
	End
	
	Event	
		ENAME	LarmRNode
		Type	Start
		At	LarmR
	
	End

	Event	
		ENAME	RightUarmGlow
		Type	Start
		At	SpadR
		part1	:GCBodyGlow.part
		Pmagnet LarmRNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Start
		At	WepR
		part1	:GCBodyGlow.part
		Pmagnet LarmRnode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
		
	End

	Event	
		ENAME	LarmLNode
		Type	Start
		At	LarmL
	
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Start
		At	SpadL
		part1	:GCBodyGlow.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Start
		At	WepL
		part1	:GCBodyGlow.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
		
	End

	Event	
		ENAME	LleglNode
		Type	Start
		At	Llegl
	
	End

	Event	
		ENAME	LeftULegGlow
		Type	Start
		At	UlegL
		part1	:GCBodyGlow.part
		Pmagnet LleglNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Start
		At	FootL
		part1	:GCBodyGlow.part
		Pmagnet LleglNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
		
	End

	Event	
		ENAME	LlegRNode
		Type	Start
		At	LlegR
	
	End

	Event	
		ENAME	RightULegGlow
		Type	Start
		At	UlegR
		part1	:GCBodyGlow.part
		Pmagnet LlegRNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Start
		At	FootR
		part1	:GCBodyGlow.part
		Pmagnet LlegRNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 60
		
	End


End

Condition
	On	Time
	Time	0
		
	Event	
		ENAME	Vortex
		Type	Start
		At	Chest
		part1	:GCVortexSteamSmall.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Pmagnet	ChestNode
		Lifespan 70
	End

End


################################################################
######################### Vortex In ############################
################################################################

Condition
	On	Time
	Time	10
	
	Event	
		ENAME	Vortex
		Type	Start
		At	Chest
		part1	:GCVortexSteam.part
		Pmagnet	ChestNode
		Lifespan 80
	End

End

Condition
	On	Time
	Time	20
		
	Event	
		ENAME	Vortex
		Type	Start
		At	Chest
		part	:GCVortexInFast.part
		Lifespan 35
	End

End

Condition
	On	Time
	Time	0
		
	Event	
		ENAME	Vortex
		Type	Start
		At	Chest
		part1	:GCVortexGlows.part
		Pmagnet ChestNode
		#Bhvr	Behaviors/GCVortexSpin.bhvr
		#Lookat	Camera
		Lifespan 85
	End

End

Condition
	On	Time
	Time	28
		
	Event	
		ENAME	VortexFlash
		Type	Start
		At	Chest
		part1	:GCFlashLrg.part
		Lifespan 20
	End

End


End	

	