#############################################################
## BodyAura.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		ChildFX	:SlowBurn_Eyes.fx
	End

	Event
		Type	Local
		At	Neck
		part	:Smoke.part
		Part	:Fire.part
		part	:Ash.part
	End

	Event
		Type	Local
		At	UArmL
		Part	:Smoke.part
		Part	:Fire.part
		POther	UArmR
	End

	Event
		Type	Local
		At	LArmR
		part	:Smoke.part
		part	:Fire.part
		part	:Ash.part
		Part	:Smoke_Trail_Small.part
		Part	:Fire_Trail.part
		POther 	UArmR
	End

	Event
		Type	Local
		At	LarmL
		part	:Smoke.part
		part	:Fire.part
		part	:Ash.part
		Part	:Smoke_Trail_Small.part
		Part	:Fire_Trail.part
		POther	UArmL
	End

	Event
		Type	Local
		At	Chest
		part	:Smoke_Trail.part
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		part	:Smoke.part
		part	:Fire.part
		part	:Ash.part
		POther  LArmR
	End

	Event
		Type	Local
		At	HandL
		part	:Smoke.part
		part	:Fire.part
		part	:Ash.part
		POther  LArmL
	End

	Event
		Type	Local
		At	LLegL
		part	:Smoke_Big.part
		part	:Fire.part
		Part	:Smoke_Trail_Small.part
		Part	:Fire_Trail.part

		POther  ULegL
	End

	Event
		Type	Local
		At	LLegR
		part	:Smoke_Big.part
		part	:Fire.part
		Part	:Smoke_Trail_Small.part
		Part	:Fire_Trail.part

		POther  ULegR
	End

	Event
		Type	Local
		At	FootR
		part	:Smoke_Big.part
		part	:Fire.part
		part	:Ash.part
		POther	 LLegR
	End

	Event
		Type	Local
		At	FootL
		part	:Smoke_Big.part
		part	:Fire.part
		part	:Ash.part
		POther	LLegL
	End

End

#############################################################

End