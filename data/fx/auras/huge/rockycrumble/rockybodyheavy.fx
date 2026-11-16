#############################################################
## RockyBodyHeavy.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Cycle
	Time	42
	Chance	0.20

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet SpadL
		POther  SpadL
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	40
	Chance	0.20

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet SpadR
		POther  SpadR
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	54
	Chance	0.20

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet Chest
		POther  Chest
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	42
	Chance	0.10

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockyChunks01.part
		Pmagnet SpadL
		POther  SpadL
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	40
	Chance	0.10

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockyChunks01.part
		Pmagnet SpadR
		POther  SpadR
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	54
	Chance	0.10

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockyChunks01.part
		Pmagnet Chest
		POther  Chest
		Lifespan 5
	End
End

#############################################################

Condition
	On	Cycle
	Time	51
	Chance	0.20

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	56
	Chance	0.20

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	60
	Chance	0.20

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	49
	Chance	0.20

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	51
	Chance	0.10

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockyChunks01.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	56
	Chance	0.10

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockyChunks01.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	60
	Chance	0.10

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockyChunks01.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	49
	Chance	0.10

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockyChunks01.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 5
	End
End

#############################################################

Condition
	On	Cycle
	Time	57
	Chance	0.20

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet Llegl
		POther  Llegl
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	48
	Chance	0.20

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet Llegl
		POther  Llegl
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	52
	Chance	0.20

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	50
	Chance	0.20

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	57
	Chance	0.10

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockyChunks01.part
		Pmagnet Llegl
		POther  Llegl
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	48
	Chance	0.10

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockyChunks01.part
		Pmagnet Llegl
		POther  Llegl
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	52
	Chance	0.10

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockyChunks01.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	50
	Chance	0.10

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockyChunks01.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan 5
	End
End

#############################################################

End