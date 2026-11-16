#############################################################
## ElectricGremlin.fx
#############################################################

FxInfo

#########################################################
Condition
	On	Time
	Time	0

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv 	1
		Part	:Eye_Glow.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv 	2
		Part	:Eye_Glow.part
	End

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			FadeOutLength		15
		End

		Flags	AdoptParentEntity
		Lifespan 5
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Part	:Body_Glow.part

		Pmagnet SpadL
		POther  SpadL
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

		Pmagnet SpadR
		POther  SpadR

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		Part	:Body_Steam.part
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		Part	:Body_Steam.part
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:Body_Steam.part
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

		Pmagnet LarmR
		POther  LarmR

	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		Part	:Body_Steam.part
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

		Pmagnet LarmL
		POther  LarmL

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:Body_Steam.part
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

		Pmagnet LarmL
		POther  LarmL
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:Body_Glow.part
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepL
		Part	:Body_Glow.part
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		Part	:Body_Steam.part
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

		Pmagnet Llegl
		POther  Llegl
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		Part	:Body_Steam.part
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

		Pmagnet Llegl
		POther  Llegl
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		Part	:Body_Steam.part
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

		Pmagnet LlegR
		POther  LlegR
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Part	:Body_Steam.part
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

		Pmagnet LlegR
		POther  LlegR
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootL
		Part	:Body_Steam.part
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Part	:Body_Steam.part
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

	End
End

Condition
	On	Cycle
	Time	22
	Chance	.5
	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Part	:Body_Electricity_Arcs.part
		Part	:Hand_Electricity_Chain.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Electricity_Arcs2.part
		Pmagnet SpadL
		POther  SpadL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End

End

Condition
	On	Cycle
	Time	20
	Chance	.5


	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet SpadR
		POther  SpadR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End

End

Condition
	On	Cycle
	Time	34
	Chance	.55


	Event
		ENAME	HeadGlow
		Type	Local
		At	Neck
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet Chest
		POther  Chest
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End

End

Condition
	On	Cycle
	Time	31
	Chance	.55


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet LarmR
		POther  LarmR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End

End

Condition
	On	Cycle
	Time	36
	Chance	.55


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet LarmR
		POther  LarmR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End

End

Condition
	On	Cycle
	Time	40
	Chance	.55


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet LarmL
		POther  LarmL
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End

End

Condition
	On	Cycle
	Time	29
	Chance	.55


	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet LarmLNode2
		POther  LarmLNode2
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End
End


Condition
	On	Cycle
	Time	37
	Chance	.55


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part
		Pmagnet Llegl
		POther  Llegl
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End
End


Condition
	On	Cycle
	Time	28
	Chance	.55


	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part
		Pmagnet Llegl
		POther  Llegl
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End
End

Condition
	On	Cycle
	Time	32
	Chance	.55

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part

		Pmagnet LlegR
		POther  LlegR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End
End


Condition
	On	Cycle
	Time	30
	Chance	.55


	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Part	:Body_Electricity_Arcs.part
		Part	:Body_Electricity_Arcs2.part
		Part	:Body_Glow_Bright.part
		Part	:Body_Sparks.part
		Pmagnet LlegR
		POther  LlegR
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 5

	End


End


End
