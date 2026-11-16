#########################################################
##	template

FxInfo


Lifespan 80

#########################################################
####################### AUDIO ###########################
#########################################################

Condition

	On 	Time	
	Time	0

	Event
		Type Local
		At origin
		Sound Warshadeeyes3 60 60 .72
	End

End




Condition

	On 	Time	
	Time	18

	Event
		Type Local
		At origin
		Sound teletrans4 80 80 .88
	End

End

Condition

	On 	Time	
	Time	70

	Event
		Type Local
		At origin
		Sound teletrans3 80 80 .78
	End

End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Primea
		Type	Local 
		At	T_Eyes
		BHVR	Behaviors/OffsetEyes.bhvr
		Geom	CircleOfThorns
		LifeSpan	50
	End

	Event
		Type	Local 
		At	Primea
		 AltPiv 1
		 BHVR	Behaviors/GenericParticleFade.bhvr
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\CircleOfThornsEyes_21Green.part
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\EyesMistLeftBLUE.part
		                               
		LifeSpan	50
		
	End
	
	Event
		Type	Local 
		At	Primea
		 AltPiv 2
		 BHVR	Behaviors/GenericParticleFade.bhvr
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\CircleOfThornsEyes_21Green.part
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\EyesMistLeftBLUE.part
		LifeSpan	50
	End

End

Condition
	On	Time
	Time	15
	
	Event	
		ENAME	ChestGlow
		Type	Start
		At	Hips
		part	:PeaceHeadGlow01.part
		part	:PeaceCoreWhite.part
		part	:PeaceCoreWhite2.part
		part	:PeaceTendril01.part
		Pmagnet NeckNode
		Bhvr	Behaviors/TelportationFade.bhvr
	Lifespan 20
	
	End

End

Condition
	On	Time
	Time	20

	Event	
		ENAME	ChestNode
		Type	Start
		At	Chest
	
	End

	Event	
		ENAME	HeadGlow
		Type	Start
		At	Hair
		part	:PeaceHeadGlow01.part
		part	:PeaceCoreWhite.part
		part	:PeaceCoreWhite2.part
		part	:PeaceTendril01.part
		Pmagnet ChestNode
		Bhvr	Behaviors/TelportationFade.bhvr
	Lifespan 20
		
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
		part	:PeaceHeadGlow01.part
		part	:PeaceCoreWhite.part
		part	:PeaceCoreWhite2.part
		part	:PeaceTendril01.part
		Pmagnet LarmRNode
		Bhvr	Behaviors/TelportationFade.bhvr
	Lifespan 20
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Start
		At	WepR
		part	:PeaceHeadGlow01.part
		part	:PeaceCoreWhite.part
		part	:PeaceCoreWhite2.part
		part	:PeaceTendril01.part
		Pmagnet LarmRnode
		Bhvr	Behaviors/TelportationFade.bhvr
	Lifespan 20
		
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
		part	:PeaceHeadGlow01.part
		part	:PeaceCoreWhite.part
		part	:PeaceCoreWhite2.part
		part	:PeaceTendril01.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/TelportationFade.bhvr
	Lifespan 20
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Start
		At	WepL
		part	:PeaceHeadGlow01.part
		part	:PeaceCoreWhite.part
		part	:PeaceCoreWhite2.part
		part	:PeaceTendril01.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/TelportationFade.bhvr
	Lifespan 20
		
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
		part1	:PeaceHeadGlow01.part
		part	:PeaceCoreWhite.part
		part	:PeaceCoreWhite2.part
		part	:PeaceTendril01.part
		Pmagnet LleglNode
		Bhvr	Behaviors/TelportationFade.bhvr
	Lifespan 20
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Start
		At	FootL
		part1	:PeaceHeadGlow01.part
		part	:PeaceCoreWhite.part
		part	:PeaceCoreWhite2.part
		part	:PeaceTendril01.part
		Pmagnet LleglNode
		Bhvr	Behaviors/TelportationFade.bhvr
	Lifespan 20
		
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
		part1	:PeaceHeadGlow01.part
		part	:PeaceCoreWhite.part
		part	:PeaceCoreWhite2.part
		part	:PeaceTendril01.part
		Pmagnet LlegRNode
		Bhvr	Behaviors/TelportationFade.bhvr
	Lifespan 20
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Start
		At	FootR
		part1	:PeaceHeadGlow01.part
		part	:PeaceCoreWhite.part
		part	:PeaceCoreWhite2.part
		part	:PeaceTendril01.part
		Pmagnet LlegRNode
		Bhvr	Behaviors/TelportationFade.bhvr
	Lifespan 20
		
	End


End

#########################################################
################## WARP OUT FLASH #######################
#########################################################


Condition
	On	Time
	Time	20
	Event	
		ENAME	StarFlash0
		Type	local
		At	Chest
		part1	:TSWarpStar01.part
		Lifespan 7
	End

End


#########################################################
################## WARP IN FLASH ########################
#########################################################


Condition
	On	Time
	Time	40

	Event
		EName 	Hurricane
		Type	local 
		At	root
		BHVR	Behaviors/PortalPivot.bhvr
		Part	:BlackHole.part
		Part	:BlackHolePurple.part
		Part	:BlackHoleBlue.part
		Part	:BlackHoleOutterRings.part
		Part	:BlackHoleOutterRingsIn.part
		Part	:PeacePortalThickness.part
		Part	:PeaceInwardPortalMatter.part
		Part	:BlackHoleCore.part

	End

End

Condition
	On	Time
	Time	56
		
	Event	
		ENAME	StarFlash0
		Type	local
		At	Chest
		part1	:TSWarpStar01.part
		Lifespan 7
	End


End


End		