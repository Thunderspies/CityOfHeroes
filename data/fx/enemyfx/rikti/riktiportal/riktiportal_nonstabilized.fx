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
		EName	AntiMatter
		Type	local
		At	Offset
		Part3	:Unstable_DarkSpecks1.part
		#Part4	:Unstable_DarkStreaks1.part
		Part	:Unstable_Iris1.part
	End

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
		EName	LightnintArcs
		Type	Local
		At	LightningArc1_Spin
		part	:Unstable_EdgeRings1.part
		part	:Unstable_EdgeRings2.part
	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.125

	Event
		EName	LightnintArcs
		Type	Local
		At	Offset
		part	:Unstable_OuwardPulseRings1.part
		Lifespan	2
	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.125

	Event
		EName	LightnintArcs
		Type	Local
		At	Offset
		part	:Unstable_Flash1.part
		Lifespan	1
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
			PositionOffset	0 0 15
		End
#		Geom	GEO_FX_TestSphere
	End

	Event
		Ename	LightningArc2_Offset
		Type	Local
		At	LightningArc1_Spin
		BhvrOverride
			PositionOffset	0 0 -15
		End
#		Geom	GEO_FX_TestSphere
	End

	Event
		Ename	LightningArc3_Offset
		Type	Local
		At	LightningArc2_Spin
		BhvrOverride
			PositionOffset	15 0 0
		End
#		Geom	GEO_FX_TestSphere
	End

	Event
		Ename	LightningArc4_Offset
		Type	Local
		At	LightningArc2_Spin
		BhvrOverride
			PositionOffset	-15 0 0
		End
#		Geom	GEO_FX_TestSphere
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
		Lifespan	2
		LifespanJitter	1
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
		Lifespan	2
		LifespanJitter	1
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
		Lifespan	2
		LifespanJitter	1
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
		Lifespan	2
		LifespanJitter	1
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
		Lifespan	2
		LifespanJitter	1
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
		Lifespan	2
		LifespanJitter	1
		pOther	LightningArc1_Offset
	End
End

#############################################################

End