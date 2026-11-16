#############################################################
## Captured Penelope Yin FX
#############################################################

FxInfo

## FX ON PENELOPE ###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	FW_PosessedConduitFloat_Loop 50 50 .5
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hair
		#Part1	:BodyGlow_Medium1.part
		Part1	:BodyGlow_Medium2.part
		pOther	Neck
	End

	Event
		Type	Local
		At	Chest
		Part1	:BodyGlow_Large1.part
		Part1	:BodyGlow_Large2.part
		Part1	:BodyGlow_Rays1.part
		Part1	:BodyGlow_Rays2.part
		pOther	Chest
	End

	Event
		Type	Local
		At	LArmL
		#Part1	:BodyGlow_Medium1.part
		Part1	:BodyGlow_Medium2.part
		pOther	UArmL
	End

	Event
		Type	Local
		At	LArmR
		#Part1	:BodyGlow_Medium1.part
		Part1	:BodyGlow_Medium2.part
		pOther	UArmR
	End

	Event
		Type	Local
		At	HandL
		Part1	:BodyGlow_Medium1.part
		Part1	:BodyGlow_Medium2.part
		pOther	LArmL
	End

	Event
		Type	Local
		At	ULegR
		Part1	:BodyGlow_Medium1.part
		Part1	:BodyGlow_Medium2.part
		pOther	ULegL
	End

	Event
		Type	Local
		At	Hips
		Part1	:BodyGlow_Large1.part
		Part1	:BodyGlow_Large2.part
		pOther	Hips
	End

	Event
		Type	Local
		At	LLegL
		Part1	:BodyGlow_Medium1.part
		Part1	:BodyGlow_Medium2.part
		pOther	ULegL
	End

	Event
		Type	Local
		At	LLegR
		Part1	:BodyGlow_Medium1.part
		Part1	:BodyGlow_Medium2.part
		pOther	ULegR
	End

	Event
		Type	Local
		At	FootL
		Part1	:BodyGlow_Medium1.part
		Part1	:BodyGlow_Medium2.part
		pOther	LLegL
	End

	Event
		Type	Local
		At	FootR
		Part1	:BodyGlow_Medium1.part
		Part1	:BodyGlow_Medium2.part
		pOther	LLegR
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	GodRayOffset1
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 128 0
		End
	End

	Event
		Type	PositOnly
		At	Hips
		part	:Cage_GodRay1.part
		pOther	GodRayOffset1
	End
End

#############################################################

End