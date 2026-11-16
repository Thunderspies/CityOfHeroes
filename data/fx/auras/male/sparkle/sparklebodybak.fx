#############################################################
## Header
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Cycle
	Time	22
	Chance .30

	Event
		ENAME	HeadNode
		Type	Local
		At	Head
		Bhvr	:HeadOffset1.bhvr
		Lifespan 5
	End

	Event
		Type	Local
		At	HeadNode
		part	:BodySparkle01.part
		Pmagnet Head
		Lifespan 5
	End

	Event
		Type	Local
		At	UarmR
		part	:BodySparkle01.part
		Pmagnet UarmR
		Lifespan 5
	End

	Event
		Type	Local
		At	FootR
		part	:BodySparkle01.part
		Pmagnet FootR
		Lifespan 5
	End

	Event
		Type	Local
		At	FootL
		part	:BodySparkle01.part
		Pmagnet FootL
		Lifespan 5
	End


	Event
		Type	Local
		At	UarmL
		part	:BodySparkle01.part
		Pmagnet UarmL
		Lifespan 5
	End


	Event
		ENAME	NeckNode
		Type	Local
		At	Neck
		Lifespan 5
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:BodySparkle01.part
		Pmagnet NeckNode
		POther  NeckNode
		Lifespan 5

	End

End

Condition
	On	Cycle
	Time	24
	Chance .30


	Event
		ENAME	HipsNode
		Type	Local
		At	Hips
		Lifespan 5

	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:BodySparkle01.part
		Pmagnet HipsNode
		POther  HipsNode
		Lifespan 5

	End

End

Condition
	On	Cycle
	Time	26
	Chance .30


	Event
		ENAME	SpadLNode
		Type	Local
		At	UArmL
		Lifespan 5
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:BodySparkle01.part
		Pmagnet SpadLNode
		POther  SpadLNode
		Lifespan 5

	End

End


Condition
	On	Cycle
	Time	28
	Chance .30


	Event
		ENAME	SpadRNode
		Type	Local
		At	UarmR
		Lifespan 5
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:BodySparkle01.part
		Pmagnet SpadRNode
		POther  SpadRNode
		Lifespan 5
	End

End

Condition
	On	Cycle
	Time	30
	Chance .30


	Event
		ENAME	ChestNode
		Type	Local
		At	Chest
		Lifespan 5

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:BodySparkle01.part
		Pmagnet ChestNode
		POther  ChestNode
		Lifespan 5



	End

End

Condition
	On	Cycle
	Time	23
	Chance .30


	Event
		ENAME	LarmRNode
		Type	Local
		At	LarmR
		Lifespan 5

	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:BodySparkle01.part
		Pmagnet LarmRNode
		POther  LarmRNode
		Lifespan 5



	End

End

Condition
	On	Cycle
	Time	25
	Chance .30


	Event
		ENAME	LarmRNode2
		Type	Local
		At	LarmR
		Lifespan 5

	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:BodySparkle01.part
		Pmagnet LarmRnode2
		POther  LarmRNode2
		Lifespan 5

	End

End

Condition
	On	Cycle
	Time	27
	Chance .30


	Event
		ENAME	LarmLNode
		Type	Local
		At	LarmL
		Lifespan 5
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:BodySparkle01.part
		Pmagnet LarmLNode
		POther  LarmLNode
		Lifespan 5


	End

End

Condition
	On	Cycle
	Time	29
	Chance .30


	Event
		ENAME	LarmLNode2
		Type	Local
		At	LarmL
		Lifespan 5
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:BodySparkle01.part
		Pmagnet LarmLNode2
		POther  LarmLNode2
		Lifespan 5


	End
End

Condition
	On	Cycle
	Time	31
	Chance .30


	Event
		ENAME	LleglNode
		Type	Local
		At	Llegl
		Lifespan 5
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:BodySparkle01.part
		Pmagnet LleglNode
		POther  LleglNode
		Lifespan 5


	End
End



Condition
	On	Cycle
	Time	32
	Chance .30

	Event
		ENAME	LlegRNode
		Type	Local
		At	LlegR
		Lifespan 5
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:BodySparkle01.part
		Pmagnet LlegRNode
		POther  LlegRNode
		Lifespan 5
	End
End

Condition
	On	Cycle
	Time	30
	Chance .30

	Event
		Type	Local
		At	LlegR
		part	:BodySparkle01.part
		Pmagnet FootR
		POther  FootR
		Lifespan 5
	End

End

Condition
	On	Cycle
	Time	28
	Chance .30

	Event
		Type	Local
		At	LlegL
		part	:BodySparkle01.part
		Pmagnet FootL
		POther  FootL
		Lifespan 5
	End

End



End