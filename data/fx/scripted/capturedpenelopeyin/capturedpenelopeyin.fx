#############################################################
## Captured Penelope Yin FX
#############################################################

FxInfo

#LifeSpan 0

## FX ON PENELOPE ###########################################################


Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	FW_CapturedApparitions_Loop 50 50 .8
	End
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Bondage
		Type	Local
		At	Waist
		Part1	:Shackle_Ring1.part
		Part1	:Shackle_Ring2.part
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	WaistSpin
		Type	Local
		At	Waist
		BhvrOverride
			Spin	0 0.125 0
		End
	End

	Event
		EName	WaistOffset1
		Type	Local
		At	WaistSpin
		BhvrOverride
			PositionOffset	1.25 0 0
		End
		#Geom	GEO_FX_TestSphere
	End

	Event
		EName	WaistOffset2
		Type	Local
		At	WaistSpin
		BhvrOverride
			PositionOffset	-0.75 0 -1
		End
		#Geom	GEO_FX_TestSphere
	End

	Event
		EName	WaistOffset3
		Type	Local
		At	WaistSpin
		BhvrOverride
			PositionOffset	-0.75 0 1
		End
		#Geom	GEO_FX_TestSphere
	End
End

## FX ON PENELOPE ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Part1	:BodyGlow_Large1.part
		Part1	:BodyGlow_Large2.part
		Part1	:BodyGlow_Rays1.part
		Part1	:BodyGlow_Rays2.part
		pMagnet	Chest
	End

	Event
		Type	Local
		At	Hips
		Part1	:BodyGlow_Large1.part
		Part1	:BodyGlow_Large2.part
	End

	Event
		Type	Local
		At	LLegL
		Part1	:BodyGlow_Medium1.part
		Part1	:BodyGlow_Medium2.part
		pMagnet	ULegL
	End

	Event
		Type	Local
		At	LLegR
		Part1	:BodyGlow_Medium1.part
		Part1	:BodyGlow_Medium2.part
		pMagnet	ULegR
	End

	Event
		Type	Local
		At	FootL
		Part1	:BodyGlow_Medium1.part
		Part1	:BodyGlow_Medium2.part
		pMagnet	LLegL
	End

	Event
		Type	Local
		At	FootR
		Part1	:BodyGlow_Medium1.part
		Part1	:BodyGlow_Medium2.part
		pMagnet	LLegR
	End

	Event
		Type	Local
		At	HandL
		Part1	:BodyGlow_Medium1.part
		Part1	:BodyGlow_Medium2.part
		pMagnet	LArmL
	End

	Event
		Type	Local
		At	HandR
		Part1	:BodyGlow_Medium1.part
		Part1	:BodyGlow_Medium2.part
		pMagnet	LArmR
	End

	Event
		Type	Local
		At	Head
		Part1	:BodyGlow_Medium1.part
		Part1	:BodyGlow_Medium2.part
	End
End

## GROUND CAGE ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	CageSpin
		Type	Local
		At	Root
		BhvrOverride
			Spin	0 0.125 0
		End
	End

	Event
		EName	CageOffset1
		Type	Local
		At	CageSpin
		BhvrOverride
			PositionOffset	2 0 0
		End
		#Geom	GEO_FX_TestSphere
	End

	Event
		EName	CageOffset2
		Type	Local
		At	CageSpin
		BhvrOverride
			PositionOffset	-1.25 0 -1.875
		End
		#Geom	GEO_FX_TestSphere
	End

	Event
		EName	CageOffset3
		Type	Local
		At	CageSpin
		BhvrOverride
			PositionOffset	-1.25 0 1.875
		End
		#Geom	GEO_FX_TestSphere
	End

	Event
		Type	Local
		At	CageOffset1
		Part1	:Cage_Rays1.part
		Part1	:Cage_Rays2.part
	End

	Event
		Type	Local
		At	Root
		part	:Cage_BaseRing1.part
		part	:Cage_BaseRing2.part
		part	:Cage_ContractingRing1.part
		part	:Cage_ContractingRing2.part
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	GodRayOffset1
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 32 0
		End
	End

	Event
		Type	Local
		At	GodRayOffset1
		part	:Cage_GodRay1.part
		pOther	Root
	End
End

Condition
	On	Cycle
	Time	10
	Chance	0.125

	Event
		Type	Local
		At	Root
		part	:Cage_ExpandingRing1.part
		part	:Cage_ExplandingFlash1.part
		Sound 	FW_CapturedApparitions_RingOut_01 50 50 .6
		Sound 	FW_CapturedApparitions_RingOut_02 50 50 .6
		Sound 	FW_CapturedApparitions_RingOut_03 50 50 .6
		Lifespan	1
	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.125

	Event
		Type	Local
		At	Root
		part	:Cage_SymbolRing1.part
		Lifespan	1
	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.125

	Event
		Type	Local
		At	Root
		part	:Cage_SymbolRing2.part
		Lifespan	1
	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.125

	Event
		Type	Local
		At	Root
		part	:Cage_SymbolRing3.part
		Sound 	FW_CapturedApparitions_RingIn_01 50 50 .8
		Lifespan	1
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.25

	Event
		Type	Local
		At	CageOffset1
		Part1	:Cage_Symbols1.part
		Lifespan	1
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.25

	Event
		Type	Local
		At	CageOffset1
		Part1	:Cage_Symbols2.part
		Lifespan	1
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.25

	Event
		Type	Local
		At	CageOffset1
		Part1	:Cage_Symbols3.part
		Lifespan	1
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.25

	Event
		Type	Local
		At	CageOffset1
		Part1	:Cage_Symbols4.part
		Lifespan	1
	End
End

#############################################################

Condition
	On 	Cycle
	Time 	5
	Chance	0.25

	Event
		Type	Posit
		At	WaistOffset1
		Part1	ENEMYFX\Rikti\RiktiPortalStabilizer\SmallBeam01.part
		Part1	ENEMYFX\Rikti\RiktiPortalStabilizer\SmallBeam02.part
		Part1	ENEMYFX\Rikti\RiktiPortalStabilizer\SmallBeamGlow1.part
		pOther	CageOffset1
		Lifespan	3
	End
End

Condition
	On 	Cycle
	Time 	5
	Chance	0.25

	Event
		Type	Posit
		At	WaistOffset2
		Part1	ENEMYFX\Rikti\RiktiPortalStabilizer\SmallBeam01.part
		Part1	ENEMYFX\Rikti\RiktiPortalStabilizer\SmallBeam02.part
		Part1	ENEMYFX\Rikti\RiktiPortalStabilizer\SmallBeamGlow1.part
		pOther	CageOffset2
		Lifespan	3
	End
End

Condition
	On 	Cycle
	Time 	5
	Chance	0.25

	Event
		Type	Posit
		At	WaistOffset3
		Part1	ENEMYFX\Rikti\RiktiPortalStabilizer\SmallBeam01.part
		Part1	ENEMYFX\Rikti\RiktiPortalStabilizer\SmallBeam02.part
		Part1	ENEMYFX\Rikti\RiktiPortalStabilizer\SmallBeamGlow1.part
		pOther	CageOffset3
		Lifespan	3
	End
End

#############################################################

End