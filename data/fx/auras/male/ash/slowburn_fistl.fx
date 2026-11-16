#############################################################
## BodyAura.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	RightLarmGlow
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
		Type	Local
		At	WepL
		part	:Smoke.part
		part	:Ash.part
	End
End

#############################################################

End