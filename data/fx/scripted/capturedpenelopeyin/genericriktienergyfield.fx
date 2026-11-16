#############################################################
## GenericRiktiEnergyField.fx
#############################################################

FxInfo

#############################################################

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
	End

	Event
		EName	WaistOffset2
		Type	Local
		At	WaistSpin
		BhvrOverride
			PositionOffset	-0.75 0 -1
		End
	End

	Event
		EName	WaistOffset3
		Type	Local
		At	WaistSpin
		BhvrOverride
			PositionOffset	-0.75 0 1
		End
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
	End

	Event
		EName	CageOffset2
		Type	Local
		At	CageSpin
		BhvrOverride
			PositionOffset	-1.25 0 -1.875
		End
	End

	Event
		EName	CageOffset3
		Type	Local
		At	CageSpin
		BhvrOverride
			PositionOffset	-1.25 0 1.875
		End
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
	On	Cycle
	Time	10
	Chance	0.125

	Event
		Type	Local
		At	Root
		part	:Cage_ExpandingRing1.part
		part	:Cage_ExplandingFlash1.part
		Lifespan 1
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
		Lifespan 1
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
		Lifespan 1
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
		Lifespan 1
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
		Lifespan 1
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
		Lifespan 1
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
		Lifespan 1
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
		Lifespan 3
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
		Lifespan 3
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
		Lifespan 3
	End
End

#############################################################

End