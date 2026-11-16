#############################################################
## ShadowyPresence_Continuing.fx
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	DarkLight
		Type	SetLight
		BHVR	:LightFade_Dark.bhvr
		Lifespan 300
	End
End

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Type	Local
#		At	Root
#		Bhvr	:FadeToTransparent.bhvr
#		Flags	AdoptParentEntity
#		Lifespan 60
#	End
#End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	LLegL
		part	:ShadowySmoke1_Small.part
		part	:ShadowySmoke2_Small.part
		sound	ShadowyPresence_ON_01 20 15 2
		pOther	FootL
	End

	Event
		Type	Local
		At	LLegR
		part	:ShadowySmoke1_Small.part
		part	:ShadowySmoke2_Small.part
		pOther	FootR
	End
End

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	ULegL
		part	:ShadowySmoke1.part
		part	:ShadowySmoke2.part
		pOther	LLegL
	End

	Event
		Type	Local
		At	ULegR
		part	:ShadowySmoke1.part
		part	:ShadowySmoke2.part
		pOther	LLegR
	End
End

Condition
	On	Time
	Time	50

	Event
		Type	Local
		At	UArmL
		part	:ShadowySmoke1.part
		part	:ShadowySmoke2.part
		pOther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		part	:ShadowySmoke1.part
		part	:ShadowySmoke2.part
		pOther	LArmR
	End

	Event
		Type	Local
		At	Hair
		part	:ShadowySmoke1.part
		part	:ShadowySmoke2.part
		pOther	Neck
	End
End

Condition
	On	Time
	Time	75

	Event
		Type	Local
		At	LArmL
		part	:ShadowySmoke1_Small.part
		part	:ShadowySmoke2_Small.part
		sound	ShadowyPresence_Continuous_Loop 20 15 0.25
		pOther	HandL
	End

	Event
		Type	Local
		At	LArmR
		part	:ShadowySmoke1_Small.part
		part	:ShadowySmoke2_Small.part
		pOther	HandR
	End
End

Condition
	On	Time
	Time	100

	Event
		Type	Local
		At	HandL
		part	:ShadowySmoke1_Small.part
		part	:ShadowySmoke2_Small.part
	End

	Event
		Type	Local
		At	HandR
		part	:ShadowySmoke1_Small.part
		part	:ShadowySmoke2_Small.part
	End
End

#############################################################

End