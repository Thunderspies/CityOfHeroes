#############################################################
## BodyAura.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		part	:Smoke_Big.part
		part	:Ash.part
		Part	:Fire_Big.part
		Part	:Smoke_Trail_Small.part
		Part	:Fire_Trail.part
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:Smoke.part
		part	:Ash.part
		Part	:Fire.part
		Part	:Smoke_Trail_Small.part
		Part	:Fire_Trail.part
		POther  LArmR
	End

	Event
		Type	Local
		At	WepL
		part	:Smoke.part
		part	:Ash.part
		Part	:Fire.part
		Part	:Smoke_Trail_Small.part
		Part	:Fire_Trail.part
		POther  LArmL
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:Smoke.part
		part	:Ash.part
	End

	Event
		Type	Local
		At	WepL
		part	:Smoke.part
		part	:Ash.part
	End
End

#############################################################

End