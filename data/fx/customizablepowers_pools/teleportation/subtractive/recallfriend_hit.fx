#############################################################
## RecallFriend_Hit.fx
#############################################################

FxInfo
Lifespan 300

#############################################################

Input
	InpName	Hips
End

#############################################################


#########################################################
####################### AUDIO ###########################
#########################################################

Condition

	On 	Time
	Time	40

	Event
		Type Local
		At origin
		Sound teletrans3 80 80 .44
	End

End

Condition

	On 	Time
	Time	41

	Event
		Type Local
		At Target
		Sound teletrans3 80 80 .44
	End

End                          



#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	5

	Event
		ENAME	ChestGlow
		Type	Start
		At	Hips
		part1	:TSChestGlow01.part
		Pmagnet Neck
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	HeadGlow
		Type	Start
		At	Hair
		part1	:TSHeadGlowTeam02.part
		Pmagnet Chest
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	RightUarmGlow
		Type	Start
		At	SpadR
		part1	:TSHeadGlowTeam02.part
		Pmagnet LarmR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	RightLarmGlow
		Type	Start
		At	WepR
		part1	:TSHeadGlowTeam02.part
		Pmagnet LarmR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftUarmGlow
		Type	Start
		At	SpadL
		part1	:TSHeadGlowTeam02.part
		Pmagnet LarmL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Start
		At	WepL
		part1	:TSHeadGlowTeam02.part
		Pmagnet LarmL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftULegGlow
		Type	Start
		At	UlegL
		part1	:TSHeadGlowTeam02.part
		Pmagnet Llegl
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLLegGlow
		Type	Start
		At	FootL
		part1	:TSHeadGlowTeam02.part
		Pmagnet Llegl
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	RightULegGlow
		Type	Start
		At	UlegR
		part1	:TSHeadGlowTeam02.part
		Pmagnet LlegR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	RightLLegGlow
		Type	Start
		At	FootR
		part1	:TSHeadGlowTeam02.part
		Pmagnet LlegR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End


End


#########################################################
################## WARP OUT FLASH #######################
#########################################################


Condition
	On	Time
	Time	18

	Event
		ENAME	StarFlash0
		Type	Start
		At	Chest
		#part1	:TSWarpStarTeam01.part
		Lifespan 1
	End

End

Condition
	On	Time
	Time	25

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
	Time	25

	Event
		ENAME	FlashRays
		Type	Start
		At	Chest
		#part1	:TSWarpFlashTeam01.part
		Pmagnet Chest
		Lifespan 1
	End

End

Condition
	On	Time
	Time	25

	Event
		ENAME	StarFlash2
		Type	Start
		At	Chest
		#part1	:TSStar02.part
		Pmagnet Chest
		Lifespan 1
	End

End



#########################################################
################## WARP IN FLASH ########################
#########################################################


Condition
	On	Time
	Time	40

	Event
		ENAME	Prime
		Type	Start
		At	Target
		Geom	RoottoChestAdjustment
	End

	Event
		ENAME	VortexGlows
		Type	Local
		At	Prime
		AltPiv	1
		part1	:TSWarpStar01.part
		Lifespan 18
	End
End

Condition
	On	Time
	Time	45

	Event
		ENAME	Vortex
		Type	Local
		At	Prime
		AltPiv	1
		part1	:TSWarpGlow01.part
		Lifespan 10
	End

End

Condition
	On	Time
	Time	45

	Event
		ENAME	VortexFlash
		Type	Start
		At	Prime
		AltPiv	1
		part1	:TSWarpFlash01.part
		Lifespan 10
	End

End

End