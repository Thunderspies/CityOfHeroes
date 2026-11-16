#########################################################
##	template

FxInfo


Lifespan 60



#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition

	On 	Time	
	Time	0

	Event
		Type Local
		At origin
		Sound teletrans4 80 80 .88
	End
End

Condition
	On	Time
	Time	10
	
	Event	
		ENAME	NeckNode
		Type	Local
		At	Neck
	
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:GCBodyGlow.part
		Pmagnet NeckNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
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
		part1	:GCBodyGlow.part
		Pmagnet ChestNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
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
		part1	:GCBodyGlow.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:GCBodyGlow.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End

End

#########################################################
################## BODY GLOW MIST #######################
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
		part1	:GCBodyGlowMist.part
		Pmagnet NeckNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
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
		part1	:GCBodyGlowMist.part
		Pmagnet ChestNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
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
		part1	:GCBodyGlowMist.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:GCBodyGlowMist.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Lifespan 0
		
	End

End


End		