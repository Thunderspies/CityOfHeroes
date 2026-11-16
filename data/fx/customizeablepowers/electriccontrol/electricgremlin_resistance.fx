#############################################################
## ElectricGremlin.fx
#############################################################

FxInfo

#########################################################
Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Part	:Birth_Shockwave.part
		Part	:Birth_Flash_Big.part
		Part	:Birth_Shockwave_Highlight.part
		SoundNoRepeat 1
		Sound EC_Gremlins_Spawn_01 50 50 .5
		Sound EC_Gremlins_Spawn_02 50 50 .5
		Lifespan 5
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		Sound EC_Gremlins_Continuing_Loop 50 50 .25
		Part	:Body_Steam.part
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Hand_Electricity_Arcs_Consistant.part
	End

	Event
		ENAME	NeckGlow
		Type	Local
		At	Neck
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part
		Pmagnet Chest
		POther  Chest
	End

	Event
		ENAME	HipsGlow
		Type	Local
		At	Hips
		Part	:Body_Glow.part
		Part	:Body_Steam.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part
		Pmagnet UArmL
		POther  UArmL
	End

	Event
		ENAME	HipsGlow2
		Type	Local
		At	Hips
		Part	:Body_Glow.part
		Part	:Body_Steam.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part
		Pmagnet UArmR
		POther  UArmR
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part
		Pmagnet UArmL
		POther  UArmL
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Chest
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

		Pmagnet UArmR
		POther  UArmR

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
		ENAME	RightHandGlow
		Type	Local
		At	HandR
		Part	:Body_Glow.part
	End

	Event
		ENAME	LeftHandGlow
		Type	Local
		At	HandL
		Part	:Body_Glow.part


	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LLegL
		Part	:Body_Steam.part
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

		Pmagnet Hips
		POther  Hips
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
		At	LLegR
		Part	:Body_Steam.part
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

		Pmagnet Hips
		POther  Hips
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
		ENAME	LeftFootGlow
		Type	Local
		At	FootL
		Part	:Body_Steam.part
		Part	:Body_Glow.part
		Part	:Body_Electricity_Arcs_Consistant.part
		Part	:Body_Electricity_Arcs_Consistant_Inverted.part

	End

	Event
		ENAME	RightFootGlow
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
		ENAME	HipsCycle
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
		ENAME	HipsCycle2
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
		ENAME	NeckCycle
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
		ENAME	UArmRCycle
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
		ENAME	RightHandCycle
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
		ENAME	UArmLCycle
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
		ENAME	LeftHandCycle
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
		ENAME	ULegLCycle
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
		ENAME	LeftFootCycle
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
		ENAME	ULegRCycle
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
		ENAME	RightFootCycle
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


Condition
	On	DEATH

	Event
		EName	GremlinDeath
		Type	Start
		At	Root
		ChildFX	:Death_Gremlin.fx
	End

	Event
		EName	HeadGlow
		Type	Destroy
	End

	Event
		EName	NeckGlow
		Type	Destroy
	End

	Event
		EName	HipsGlow
		Type	Destroy
	End

	Event
		EName	HipsGlow2
		Type	Destroy
	End

	Event
		EName	ChestGlow
		Type	Destroy
	End

	Event
		EName	ChestGlow2
		Type	Destroy
	End

	Event
		EName	RightUarmGlow
		Type	Destroy
	End

	Event
		EName	RightLarmGlow
		Type	Destroy
	End

	Event
		EName	LeftUarmGlow
		Type	Destroy
	End

	Event
		EName	LeftLarmGlow
		Type	Destroy
	End

	Event
		EName	RightHandGlow
		Type	Destroy
	End

	Event
		EName	LeftHandGlow
		Type	Destroy
	End


	Event
		EName	RightULegGlow
		Type	Destroy
	End

	Event
		EName	RightLLegGlow
		Type	Destroy
	End

	Event
		EName	LeftULegGlow
		Type	Destroy
	End

	Event
		EName	LeftLLegGlow
		Type	Destroy
	End

	Event
		EName	LeftFootGlow
		Type	Destroy
	End

	Event
		EName	RightFootGlow
		Type	Destroy
	End

	Event
		EName	HipsCycle
		Type	Destroy
	End

	Event
		EName	HipsCycle2
		Type	Destroy
	End

	Event
		EName	NeckCycle
		Type	Destroy
	End

	Event
		EName	UArmRCycle
		Type	Destroy
	End

	Event
		EName	UArmLCycle
		Type	Destroy
	End

	Event
		EName	RightHandCycle
		Type	Destroy
	End

	Event
		EName	LeftHandCycle
		Type	Destroy
	End

	Event
		EName	ULegLCycle
		Type	Destroy
	End

	Event
		EName	ULegRCycle
		Type	Destroy
	End

	Event
		EName	RightFootCycle
		Type	Destroy
	End

	Event
		EName	LeftFootCycle
		Type	Destroy
	End
End

#########################################################

End
