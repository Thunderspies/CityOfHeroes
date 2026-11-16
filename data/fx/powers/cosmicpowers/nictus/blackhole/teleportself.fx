#########################################################
## Warshade - Shadow Step
#########################################################

FxInfo

Lifespan 165

## SOUND FX ######################################################

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

## EYE FX ######################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Eyes1
		Type	Local 
		At	Root
		ChildFX POWERS\CosmicPowers\Nictus\ActivationEyes\ActivationEyes.fx
	End
End

Condition
	On	Time
	Time	18

	Event
		EName 	Eyes1
		Type	Destroy
	End
End

Condition
	On	Time
	Time	60

	Event
		EName 	Eyes2
		Type	Local 
		At	Root
		ChildFX POWERS\CosmicPowers\Nictus\ActivationEyes\ActivationEyesContinuingFX.fx
	End
End

## TELEPORT FX ######################################################

Condition
	On	Time
	Time	15
	
	Event	
		ENAME	ChestGlow
		Type	Start
		At	Hips
		part	:TSHeadGlow01.part
		part	:NictusCoreBlack.part
		part	:NictusCoreBlack2.part
		part	:PurpleTendril01.part
		Pmagnet NeckNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan	20
	End
End

Condition
	On	Time
	Time	20

	Event	
		ENAME	HeadGlow
		Type	Start
		At	Hair
		part	:TSHeadGlow01.part
		part	:NictusCoreBlack.part
		part	:NictusCoreBlack2.part
		part	:PurpleTendril01.part
		Pmagnet Chest
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan	20
	End
	
	Event	
		ENAME	RightUarmGlow
		Type	Start
		At	SpadR
		part	:TSHeadGlow01.part
		part	:NictusCoreBlack.part
		part	:NictusCoreBlack2.part
		part	:PurpleTendril01.part
		Pmagnet LarmR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan	20
	End

	Event	
		ENAME	RightLarmGlow
		Type	Start
		At	WepR
		part	:TSHeadGlow01.part
		part	:NictusCoreBlack.part
		part	:NictusCoreBlack2.part
		part	:PurpleTendril01.part
		Pmagnet LarmRnode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan	20
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Start
		At	SpadL
		part	:TSHeadGlow01.part
		part	:NictusCoreBlack.part
		part	:NictusCoreBlack2.part
		part	:PurpleTendril01.part
		Pmagnet LarmL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan	20
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Start
		At	WepL
		part	:TSHeadGlow01.part
		part	:NictusCoreBlack.part
		part	:NictusCoreBlack2.part
		part	:PurpleTendril01.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan	20
	End

	Event	
		ENAME	LeftULegGlow
		Type	Start
		At	UlegL
		part1	:TSHeadGlow01.part
		part	:NictusCoreBlack.part
		part	:NictusCoreBlack2.part
		part	:PurpleTendril01.part
		Pmagnet Llegl
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan	20
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Start
		At	FootL
		part1	:TSHeadGlow01.part
		part	:NictusCoreBlack.part
		part	:NictusCoreBlack2.part
		part	:PurpleTendril01.part
		Pmagnet LleglNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan	20
	End

	Event	
		ENAME	RightULegGlow
		Type	Start
		At	UlegR
		part1	:TSHeadGlow01.part
		part	:NictusCoreBlack.part
		part	:NictusCoreBlack2.part
		part	:PurpleTendril01.part
		Pmagnet LlegR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan	20
	End

	Event	
		ENAME	RightLLegGlow
		Type	Start
		At	FootR
		part1	:TSHeadGlow01.part
		part	:NictusCoreBlack.part
		part	:NictusCoreBlack2.part
		part	:PurpleTendril01.part
		Pmagnet LlegRNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan	20
	End
End

## WARP OUT FLASH ######################################################

Condition
	On	Time
	Time	20
	Event	
		ENAME	StarFlash0
		Type	local
		At	Chest
		part1	:TSWarpStar01.part
		Lifespan	7
	End
End

## WARP IN FLASH ######################################################

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
		Part	:PortalThickness.part
		Part	:InwardPortalMatter.part
		Part	:BlackHoleCore.part
		Lifespan	80
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
		Lifespan	7
	End
End

########################################################

End		