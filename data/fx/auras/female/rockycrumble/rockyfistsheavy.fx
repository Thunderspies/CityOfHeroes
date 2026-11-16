#############################################################
## RockyFistsHeavy.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Cycle
	Time	51
	Chance	0.50

	Event
		ENAME	RightLarmGlow
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
	Chance	0.50

	Event
		ENAME	LeftLarmGlow
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

#############################################################

End