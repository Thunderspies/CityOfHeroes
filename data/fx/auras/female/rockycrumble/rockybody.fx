#############################################################
## RockyBody.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	SpadLNode
		Type	Local
		At	SpadL
	End
End
Condition
	On	Cycle
	Time	42
	Chance	.20


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet SpadLNode
		POther  SpadLNode
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End

End


Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	SpadRNode
		Type	Local
		At	SpadR
	End
End
Condition
	On	Cycle
	Time	40
	Chance	.20


	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet SpadRNode
		POther  SpadRNode
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End

End

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	ChestNode
		Type	Local
		At	Chest

	End
End
Condition
	On	Cycle
	Time	54
	Chance	.20


	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet ChestNode
		POther  ChestNode
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End

End

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	LarmRNode
		Type	Local
		At	LarmR

	End
End
Condition
	On	Cycle
	Time	51
	Chance	.20


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet LarmRNode
		POther  LarmRNode
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End

End

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	LarmRNode2
		Type	Local
		At	LarmR

	End
End
Condition
	On	Cycle
	Time	56
	Chance	.20


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet LarmRnode2
		POther  LarmRNode2
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End

End

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	LarmLNode
		Type	Local
		At	LarmL

	End
End
Condition
	On	Cycle
	Time	60
	Chance	.20


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet LarmLNode
		POther  LarmLNode
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
	Time	49
	Chance	.20


	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet LarmLNode2
		POther  LarmLNode2
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End
End

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	LleglNode
		Type	Local
		At	Llegl

	End
End
Condition
	On	Cycle
	Time	57
	Chance	.20


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet LleglNode
		POther  LleglNode
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End
End

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	LleglNode2
		Type	Local
		At	Llegl

	End
End
Condition
	On	Cycle
	Time	48
	Chance	.20


	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet LleglNode2
		POther  LleglNode2
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End
End

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	LlegRNode
		Type	Local
		At	LlegR

	End
End
Condition
	On	Cycle
	Time	52
	Chance	.20

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet LlegRNode
		POther  LlegRNode
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End
End

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	LlegRNode2
		Type	Local
		At	LlegR

	End
End
Condition
	On	Cycle
	Time	50
	Chance	.20


	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:RockySmoke01.part
		part	:RockyDust01.part
		Pmagnet LlegRNode2
		POther  LlegRNode2
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End


End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	SpadLNode
		Type	Local
		At	SpadL
	End
End
Condition
	On	Cycle
	Time	42
	Chance	.10


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:RockyChunks01.part
		Pmagnet SpadLNode
		POther  SpadLNode
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End

End


Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	SpadRNode
		Type	Local
		At	SpadR
	End
End
Condition
	On	Cycle
	Time	40
	Chance	.10


	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:RockyChunks01.part
		Pmagnet SpadRNode
		POther  SpadRNode
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End

End

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	ChestNode
		Type	Local
		At	Chest

	End
End
Condition
	On	Cycle
	Time	54
	Chance	.10


	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:RockyChunks01.part
		Pmagnet ChestNode
		POther  ChestNode
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End

End

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	LarmRNode
		Type	Local
		At	LarmR

	End
End
Condition
	On	Cycle
	Time	51
	Chance	.10


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:RockyChunks01.part
		Pmagnet LarmRNode
		POther  LarmRNode
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End

End

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	LarmRNode2
		Type	Local
		At	LarmR

	End
End
Condition
	On	Cycle
	Time	56
	Chance	.10


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:RockyChunks01.part
		Pmagnet LarmRnode2
		POther  LarmRNode2
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End

End

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	LarmLNode
		Type	Local
		At	LarmL

	End
End
Condition
	On	Cycle
	Time	60
	Chance	.10


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:RockyChunks01.part
		Pmagnet LarmLNode
		POther  LarmLNode
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
	Time	49
	Chance	.10


	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:RockyChunks01.part
		Pmagnet LarmLNode2
		POther  LarmLNode2
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End
End

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	LleglNode
		Type	Local
		At	Llegl

	End
End
Condition
	On	Cycle
	Time	57
	Chance	.10


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:RockyChunks01.part
		Pmagnet LleglNode
		POther  LleglNode
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End
End

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	LleglNode2
		Type	Local
		At	Llegl

	End
End
Condition
	On	Cycle
	Time	48
	Chance	.10


	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:RockyChunks01.part
		Pmagnet LleglNode2
		POther  LleglNode2
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End
End

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	LlegRNode
		Type	Local
		At	LlegR

	End
End
Condition
	On	Cycle
	Time	52
	Chance	.10


	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:RockyChunks01.part
		Pmagnet LlegRNode
		POther  LlegRNode
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End
End

Condition	## Auto-extracted from the next On Cycle condition
	Event
		ENAME	LlegRNode2
		Type	Local
		At	LlegR

	End
End
Condition
	On	Cycle
	Time	50
	Chance	.10


	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:RockyChunks01.part
		Pmagnet LlegRNode2
		POther  LlegRNode2
		Bhvr	Behaviors\TelportationFade.bhvr
		Lifespan 5

	End


End

End
