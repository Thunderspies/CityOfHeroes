#############################################################
## RockyFeetHeavy.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Cycle
	Time	51
	Chance	0.5

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmokeFeet01.part
		part	:RockyDust01.part
		part	:RockyChunks01.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	60
	Chance	0.5

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:RockySmokeFeet01.part
		part	:RockyDust01.part
		part	:RockyChunks01.part
		Pmagnet LlegL
		POther  LlegL
		Lifespan 5
	End
End

#############################################################

End