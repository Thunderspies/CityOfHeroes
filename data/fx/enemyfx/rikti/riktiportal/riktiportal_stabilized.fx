#############################################################
## FX System Name
#############################################################

FxInfo


#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Offset
		Type	start
		At	Root
		BhvrOverride
			PyrRotate	90 0 0
			PositionOffset	0 17 0
		End
	End

	Event
		Type	Local
		At	Offset
		Part1	:Stabilized_SwirlingEnergy_Dark1.part
		Part1	:Stabilized_SwirlingEnergy_Dark2.part
		Part1	:Stabilized_SwirlingEnergy_Bright1.part
		Part1	:Stabilized_SwirlingEnergy_Bright2.part
	End

	Event
		EName	Rings
		Type	local
		At	Offset
		Sound glow6_loop 70 70 .67
		Part1	:Stabilized_OutwardRings1.part
		Part2	:Stabilized_OutwardRings2.part
	End

	Event
		EName	AntiMatter
		Type	local
		At	Offset
		Part3	:Stabilized_DarkSpecks1.part
		Part4	:Stabilized_DarkStreaks1.part
	End
End

Condition
	On	Cycle
	Time	45

	Event
		EName	Ring2
		Type	local
		At	Offset
		Part3	:Stabilized_PulsingRings1.part
		Lifespan	4
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	LightningArc1_Spin
		Type	Local
		At	Offset
		BhvrOverride
			Spin	0 0.125 0
		End
	End

	Event
		Ename	LightningArc2_Spin
		Type	Local
		At	Offset
		BhvrOverride
			Spin	0 -0.2 0
		End
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	LightningArc1_Offset
		Type	Local
		At	LightningArc1_Spin
		BhvrOverride
			PositionOffset	0 0 17
		End
	End

	Event
		Ename	LightningArc2_Offset
		Type	Local
		At	LightningArc1_Spin
		BhvrOverride
			PositionOffset	0 0 -17
		End
	End

	Event
		Ename	LightningArc3_Offset
		Type	Local
		At	LightningArc2_Spin
		BhvrOverride
			PositionOffset	17 0 0
		End
	End

	Event
		Ename	LightningArc4_Offset
		Type	Local
		At	LightningArc2_Spin
		BhvrOverride
			PositionOffset	-17 0 0
		End
	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.125

	Event
		EName	LightnintArcs
		Type	Start
		At	LightningArc1_Offset
		Part3	:Unstable_LightningArc1.part
		Part3	:Unstable_LightningGlow1.part
		Lifespan	1
		pOther	LightningArc2_Offset
	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.125

	Event
		EName	LightnintArcs
		Type	Start
		At	LightningArc1_Offset
		Part3	:Unstable_LightningArc2.part
		Part3	:Unstable_LightningGlow1.part
		Lifespan	1
		pOther	LightningArc2_Offset
	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.125

	Event
		EName	LightnintArcs
		Type	Start
		At	LightningArc3_Offset
		Part3	:Unstable_LightningArc1.part
		Part3	:Unstable_LightningGlow1.part
		Lifespan	1
		pOther	LightningArc4_Offset
	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.125

	Event
		EName	LightnintArcs
		Type	Start
		At	LightningArc3_Offset
		Part3	:Unstable_LightningArc2.part
		Part3	:Unstable_LightningGlow1.part
		Lifespan	1
		pOther	LightningArc4_Offset
	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.25

	Event
		EName	LightnintArcs
		Type	Start
		At	LightningArc2_Offset
		Part3	:Unstable_LightningArc1.part
		Part3	:Unstable_LightningGlow1.part
		Lifespan	1
		pOther	LightningArc3_Offset
	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.25

	Event
		EName	LightnintArcs
		Type	Start
		At	LightningArc4_Offset
		Part3	:Unstable_LightningArc2.part
		Part3	:Unstable_LightningGlow1.part
		Lifespan	1
		pOther	LightningArc1_Offset
	End
End

#############################################################

End				