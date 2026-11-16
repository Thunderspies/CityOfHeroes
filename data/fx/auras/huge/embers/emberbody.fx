#############################################################
## Header
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UarmR
		part	:BodyEmbers01.part
		part	:BodyEmbers02.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	FootR
		part	:BodyEmbers01.part
		part	:BodyEmbers02.part
		Pmagnet FootR
	End

	Event
		Type	Local
		At	FootL
		part	:BodyEmbers01.part
		part	:BodyEmbers02.part
		Pmagnet FootL
	End


	Event
		Type	Local
		At	UarmL
		part	:BodyEmbers01.part
		part	:BodyEmbers02.part
		Pmagnet UarmL
	End


	Event
		ENAME	NeckNode
		Type	Local
		At	Neck
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:BodyEmbers01.part
		part	:BodyEmbers02.part
		Pmagnet NeckNode
		POther  NeckNode

	End

End

Condition
	On	Time
	Time	0


	Event
		ENAME	HipsNode
		Type	Local
		At	Hips

	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:BodyEmbers01.part
		part	:BodyEmbers02.part
		part	:BodyEmbersTrail01.part
		Pmagnet HipsNode
		POther  HipsNode

	End

End

Condition
	On	Time
	Time	0


	Event
		ENAME	SpadLNode
		Type	Local
		At	UArmL


	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:BodyEmbers01.part
		part	:BodyEmbers02.part
		Pmagnet SpadLNode
		POther  SpadLNode

	End

End


Condition
	On	Time
	Time	0


	Event
		ENAME	SpadRNode
		Type	Local
		At	UarmR
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:BodyEmbers01.part
		part	:BodyEmbers02.part
		Pmagnet SpadRNode
		POther  SpadRNode



	End

End

Condition
	On	Time
	Time	0


	Event
		ENAME	ChestNode
		Type	Local
		At	Chest

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:BodyEmbers01.part
		part	:BodyEmbers02.part
		Pmagnet ChestNode
		POther  ChestNode



	End

End

Condition
	On	Time
	Time	0


	Event
		ENAME	LarmRNode
		Type	Local
		At	LarmR

	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:BodyEmbers01.part
		part	:BodyEmbers02.part
		Pmagnet LarmRNode
		POther  LarmRNode



	End

End

Condition
	On	Time
	Time	0


	Event
		ENAME	LarmRNode2
		Type	Local
		At	LarmR

	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:BodyEmbers01.part
		part	:BodyEmbers02.part
		Pmagnet LarmRnode2
		POther  LarmRNode2



	End

End

Condition
	On	Time
	Time	0


	Event
		ENAME	LarmLNode
		Type	Local
		At	LarmL

	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:BodyEmbers01.part
		part	:BodyEmbers02.part
		Pmagnet LarmLNode
		POther  LarmLNode



	End

End

Condition
	On	Time
	Time	0


	Event
		ENAME	LarmLNode2
		Type	Local
		At	LarmL

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:BodyEmbers01.part
		part	:BodyEmbers02.part
		Pmagnet LarmLNode2
		POther  LarmLNode2



	End
End

Condition
	On	Time
	Time	0


	Event
		ENAME	LleglNode
		Type	Local
		At	Llegl

	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:BodyEmbers01.part
		part	:LegEmbers01.part
		Pmagnet LleglNode
		POther  LleglNode



	End
End



Condition
	On	Time
	Time	0

	Event
		ENAME	LlegRNode
		Type	Local
		At	LlegR
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:BodyEmbers01.part
		part	:LegEmbers01.part
		Pmagnet LlegRNode
		POther  LlegRNode

	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	LlegR
		part	:BodyEmbers01.part
		part	:BodyEmbers02.part
		Pmagnet FootR
		POther  FootR
	End

	Event
		Type	Local
		At	LlegL
		part	:BodyEmbers01.part
		part	:BodyEmbers02.part
		Pmagnet FootL
		POther  FootL
	End

End



End