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
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:FistEmbers01.part
		part	:BodyEmbers01.part
		part	:HeadEmbersTrail01.part
		Pmagnet LarmR
		POther  LarmR



	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:FistEmbers01.part
		part	:BodyEmbers01.part
		part	:HeadEmbersTrail01.part
		Pmagnet LarmL
		POther  LarmL



	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	LarmR
		part	:BodyEmbers01.part
		Pmagnet UarmR
		POther  UarmR



	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	LarmL
		part	:BodyEmbers01.part
		Pmagnet UarmL
		POther  UarmL



	End
End



End