#############################################################
## RockyBodyLight.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Cycle
	Time	42
	Chance	0.05

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		part	:RockyChunks01.part
		Pmagnet SpadL
		POther  SpadL
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	40
	Chance	0.05

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		part	:RockyChunks01.part
		Pmagnet SpadR
		POther  SpadR
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	54
	Chance	0.05

	Event
		Type	Local
		At	Hair
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		part	:RockyChunks01.part
		Pmagnet Chest
		POther  Chest
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	51
	Chance	0.05

	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		part	:RockyChunks01.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	56
	Chance	0.05

	Event
		Type	Local
		At	HandR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		part	:RockyChunks01.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	60
	Chance	0.05

	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		part	:RockyChunks01.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	49
	Chance	0.05

	Event
		Type	Local
		At	HandL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		part	:RockyChunks01.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	57
	Chance	0.05

	Event
		Type	Local
		At	UlegL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		part	:RockyChunks01.part
		Pmagnet Llegl
		POther  Llegl
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	48
	Chance	0.05

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		part	:RockyChunks01.part
		Pmagnet Llegl
		POther  Llegl
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	52
	Chance	0.05

	Event
		Type	Local
		At	UlegR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		part	:RockyChunks01.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	50
	Chance	0.05

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmoke01.part
		part	:RockyDust01.part
		part	:RockyChunks01.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan 5
	End
End

#############################################################

End
