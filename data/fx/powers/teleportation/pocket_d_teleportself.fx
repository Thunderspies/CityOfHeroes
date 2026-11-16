#########################################################
##	template

FxInfo

Lifespan 300

#########################################################
####################### AUDIO ###########################
#########################################################

Condition

	On 	Time	
	Time	18

	Event
		Type Local
		At origin
		Sound teletrans2 80 80 .88
	End

End

Condition

	On 	Time	
	Time	10

	Event
		Type Local
		At origin
		Sound teletrans3 80 80 .7
	End

End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0	#20
	
	Event	
		ENAME	NeckNode
		Type	Start
		At	Neck

	
	End

	Event	
		ENAME	ChestGlow
		Type	Start
		At	Hips
		part1	:TSChestGlow01.part
		Pmagnet NeckNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
	
	End

	Event	
		ENAME	ChestNode
		Type	Start
		At	Chest
	
	End

	Event	
		ENAME	HeadGlow
		Type	Start
		At	Hair
		part1	:TSHeadGlow01.part
		Pmagnet ChestNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
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
		part1	:TSHeadGlow01.part
		Pmagnet LarmRNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Start
		At	WepR
		part1	:TSHeadGlow01.part
		Pmagnet LarmRnode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
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
		part1	:TSHeadGlow01.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Start
		At	WepL
		part1	:TSHeadGlow01.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
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
		part1	:TSHeadGlow01.part
		Pmagnet LleglNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Start
		At	FootL
		part1	:TSHeadGlow01.part
		Pmagnet LleglNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
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
		part1	:TSHeadGlow01.part
		Pmagnet LlegRNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Start
		At	FootR
		part1	:TSHeadGlow01.part
		Pmagnet LlegRNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


End

#########################################################
#################### BODY GLOW 2 ########################
#########################################################

Condition
	On	Time
	Time	50	#70
		
	Event	
		ENAME	ChestGlow
		Type	Start
		At	Hips
		part1	:TSHeadGlow01.part
		Pmagnet Neck
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
	
	End

	Event	
		ENAME	HeadGlow
		Type	Start
		At	Hair
		part1	:TSHeadGlow01.part
		Pmagnet Chest
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	RightUarmGlow
		Type	Start
		At	SpadR
		part1	:TSHeadGlow01.part
		Pmagnet LarmR
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	RightLarmGlow
		Type	Start
		At	WepR
		part1	:TSHeadGlow01.part
		Pmagnet LarmR
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Start
		At	SpadL
		part1	:TSHeadGlow01.part
		Pmagnet LarmL
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Start
		At	WepL
		part1	:TSHeadGlow01.part
		Pmagnet LarmL
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftULegGlow
		Type	Start
		At	UlegL
		part1	:TSHeadGlow01.part
		Pmagnet Llegl
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Start
		At	FootL
		part1	:TSHeadGlow01.part
		Pmagnet Llegl
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Start
		At	UlegR
		part1	:TSHeadGlow01.part
		Pmagnet LlegR
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Start
		At	FootR
		part1	:TSHeadGlow01.part
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
	Time	25	#45
		
	Event	
		ENAME	StarFlash0
		Type	Start
		At	Chest
		part1	:TSWarpStar01.part
		Lifespan 18
	End

End

Condition
	On	Time
	Time	33	#53
		
	Event	
		ENAME	GlowFlash
		Type	Start
		At	Chest
		part1	:TSWarpGlow01.part
		Lifespan 10
	End

End

Condition
	On	Time
	Time	33	#53
		
	Event	
		ENAME	FlashRays
		Type	Start
		At	Chest
		part1	:TSWarpFlash01.part
		Pmagnet Chest
		Lifespan 10
	End

End

Condition
	On	Time
	Time	33	#53
		
	Event	
		ENAME	StarFlash2
		Type	Start
		At	Chest
		#part1	:TSStar02.part
		Pmagnet Chest
		Lifespan 20
	End

End

#########################################################
################## WARP IN FLASH ########################
#########################################################

Condition
	On	Time
	Time	46	#66
		
	Event	
		ENAME	StarFlash0
		Type	Start
		At	Chest
		part1	:TSWarpStar01.part
		Lifespan 18
	End

End

Condition
	On	Time
	Time	50	#70
		
	Event	
		ENAME	GlowFlash
		Type	Start
		At	Chest
		part1	:TSWarpGlow01.part
		Lifespan 10
	End

End

Condition
	On	Time
	Time	50	#70
		
	Event	
		ENAME	FlashRays
		Type	Start
		At	Chest
		part1	:TSWarpFlash01.part
		Pmagnet Chest
		Lifespan 10
	End

End

End		