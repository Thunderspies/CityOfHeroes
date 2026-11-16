#############################################################
## RockyFists.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	LarmRNode2
		Type	Local
		At	LarmR

	End
End
Condition
	On	Cycle
	Time	51
	Chance	.50


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:RockySmoke01.part
		part	:RockyDust01.part
		part	:RockyChunks01.part
		Pmagnet LarmRnode2
		POther  LarmRNode2
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End

End

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	LarmLNode2
		Type	Local
		At	LarmL

	End
End
Condition
	On	Cycle
	Time	60
	Chance	.50



	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:RockySmoke01.part
		part	:RockyDust01.part
		part	:RockyChunks01.part
		Pmagnet LarmLNode2
		POther  LarmLNode2
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End
End


End
