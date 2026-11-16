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
		part	:BodySparkle01.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	UarmL
		part	:BodySparkle01.part
		Pmagnet UarmL
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
		part	:BodySparkle01.part
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
		part	:BodySparkle01.part
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
		part	:BodySparkle01.part
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
		part	:BodySparkle01.part
		Pmagnet LarmLNode2
		POther  LarmLNode2

	End
End

End