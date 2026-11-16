#########################################################
## Jacob's Ladder
########################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On 	Time
	Time 	21

	Event
		EName	Offset_ArmL
		Type	Start
		At	UArmL
		BhvrOverride
			TrackRate	0.125
		End
		LookAt	WepL
		Magnet 	WepL
		LifeSpan 30
	End

	Event
		EName	Offset_ArmR
		Type	Start
		At	UArmR
		BhvrOverride
			TrackRate	0.125
		End
		LookAt	WepR
		Magnet 	WepR
		LifeSpan 30
	End
End

## JACOBS LADDER ###########################################################

Condition
	On 	Time
	Time 	22

	Event
		EName	Electricity
		Type	local
		At	GlowL
		Part	:Electricity01.part
		Part	:Electricity01Core.part
		POther	GlowR
		LifeSpan 11
	End
End

Condition
	On 	Time
	Time 	21

	Event
		EName	GlowL
		Type	local
		At	Offset_ArmL
		Part	:ElectricityGlowLeft03.part
		Part	:ElectricityGlowLeft01.part
		Sound TeslaShotgun 70 70 0.8
		LifeSpan 12
	End

	Event
		EName	GlowR
		Type	local
		At	Offset_ArmR
		Part	:ElectricityGlowLeft03.part
		Part	:ElectricityGlowLeft01.part
		LifeSpan 12
	End
End

## ARM ELECTRICITY ###########################################################

Condition
	On 	Time
	Time 	21

	Event
		EName	GlowL
		Type	local
		At	HandL
		POther	LarmL
		Part	:ElectricityGlowArm01.part
		LifeSpan 15
	End

	Event
		EName	GlowR
		Type	local
		At	HandR
		POther	LarmR
		Part	:ElectricityGlowArm01.part
		LifeSpan 15
	End

	Event
		EName	GlowL
		Type	local
		At	UArmL
		POther	LarmL
		Part	:ElectricityGlowArm01.part
		LifeSpan 15
	End

	Event
		EName	GlowR
		Type	local
		At	UArmR
		POther	LarmR
		Part	:ElectricityGlowArm01.part
		LifeSpan 15
	End

	Event
		EName	GlowL
		Type	local
		At	WepL
		Part	:ElectricityGlowArm01.part
		LifeSpan 15
	End

	Event
		EName	GlowRS
		Type	local
		At	WepR
		Part	:ElectricityGlowArm01.part
		LifeSpan 15
	End
End

## LIGHTNING HANDS ###########################################################

Condition
	On 	Time
	Time 	36

	Event
		Type	local
		At	HandR
		BHVR	POWERS/ElectricityControl/LightningHandRotateUP.bhvr
		Part	:ElectricityOuterGlow.part
		Part	:ElectricityRing.part
		Part	:ElectricityArchWide.part
		Part	:ElectricityArchWideWhite.part
		Part	:ElectricityArchInvertWide.part
		Part	:ElectricityArchInvertWideWhite.part
		Sound eleccontrol2 50 50 0.4
		LifeSpan 20
	End

	Event
		Type	local
		At	HandL
		BHVR	POWERS/ElectricityControl/LightningHandRotateLeftUP.bhvr
		Part	:ElectricityOuterGlow.part
		Part	:ElectricityRing.part
		Part	:ElectricityArchWide.part
		Part	:ElectricityArchWideWhite.part
		Part	:ElectricityArchInvertWide.part
		Part	:ElectricityArchInvertWideWhite.part
		LifeSpan 20
	End
End

#############################################################

End