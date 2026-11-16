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

#############################################################

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

#############################################################

Condition
	On 	Time
	Time 	31

	Event
		EName	GlowL
		Type	local
		At	HandL
		POther	LarmL
		Part	:ElectricityGlowArm01.part
		LifeSpan 29
	End

	Event
		EName	GlowR
		Type	local
		At	HandR
		POther	LarmR
		Part	:ElectricityGlowArm01.part
		LifeSpan 29
	End

	Event
		EName	GlowL
		Type	local
		At	UArmL
		POther	LarmL
		Part	:ElectricityGlowArm01.part
		LifeSpan 29
	End

	Event
		EName	GlowR
		Type	local
		At	UArmR
		POther	LarmR
		Part	:ElectricityGlowArm01.part
		LifeSpan 29
	End

	Event
		EName	GlowL
		Type	local
		At	WepL
		Part	:ElectricityGlowArm01.part
		LifeSpan 30
	End

	Event
		EName	GlowR
		Type	local
		At	WepR
		Part	:ElectricityGlowArm01.part
		LifeSpan 30
	End
End

Condition
	On 	Time
	Time 	50

	Event
		EName 	Thingy
		Type	local
		At	uarmR
		Part	:JacobsladederElectricity01.part
		Part	:JacobsladederElectricity01White.part
		magnet	WepR
		LifeSpan 40
	End

	Event
		EName 	Thingy
		Type	local
		At	LarmR
		Part	:JacobsladederElectricity01.part
		Part	:JacobsladederElectricity01White.part
		magnet	WepR
		LifeSpan 40
	End

	Event
		EName 	Thingy
		Type	local
		At	uarmL
		Part	:JacobsladederElectricity02.part
		Part	:JacobsladederElectricity02White.part
		magnet	WepL
		LifeSpan 40
	End

	Event
		EName 	Thingy
		Type	local
		At	LarmL
		Part	:JacobsladederElectricity02.part
		Part	:JacobsladederElectricity02White.part
		magnet	WepL
		LifeSpan 40
	End
End

#############################################################

Condition
	On 	Time
	Time 	36

	Event
		EName 	Thingy
		Type	local
		At	HandR
		BHVR	:LightningHandRotateUP.bhvr
		Part	:WitchSparks.part
		Part1	:Red_ElectricityControl01.part
		Part2	:Red_ElectricityControl02.part
		Part1	:Red_ElectricityControl01.part
		Part2	:Red_ElectricityControl02.part
		Part1	:Red_ElectricityControl01White2.part
		Part2	:Red_ElectricityControl02White2.part
		Sound eleccontrol2 50 50 0.4
		LifeSpan 20
	End

	Event
		EName 	Thingy2
		Type	local
		At	HandL
		BHVR	:LightningHandRotateLeftUP.bhvr
		Part	:WitchSparks.part
		Part1	:Red_ElectricityControl01.part
		Part2	:Red_ElectricityControl02.part
		Part1	:Red_ElectricityControl01.part
		Part2	:Red_ElectricityControl02.part
		Part1	:Red_ElectricityControl01White2.part
		Part2	:Red_ElectricityControl02White2.part
		LifeSpan 20
	End

	Event
		EName 	Thingy3
		Type	local
		At	HandR
		BHVR	:LightningHandRotateUP.bhvr
		Part	:ElectricityOuterGlow.part
		Part	:Red_ElectricityArch.part
		Part	:ElectricityRing.part
		Part	:Red_ElectricityArchInvert.part
		Part	:Red_ElectricityArch.part
		Part	:Red_ElectricityArchInvert2.part
		Part	:Red_ElectricityArch2.part
		Part	:RedElectricityArchInvert.part
		Part	:RedElectricityArchWide.part
		Part	:RedElectricityArchInvertWide.part
		Part	:ElectricityArchWideWhite.part
		Part	:ElectricityArchInvertWideWhite.part
		Part	:ElectricityArchWideWhite2.part
		Part	:ElectricityArchInvertWideWhite2.part
		LifeSpan 20
	End

	Event
		EName 	Thingy4
		Type	local
		At	HandL
		BHVR	:LightningHandRotateLeftUP.bhvr
		Part	:ElectricityOuterGlow.part
		Part	:Red_ElectricityArch.part
		Part	:ElectricityRing.part
		Part	:Red_ElectricityArchInvert.part
		Part	:Red_ElectricityArch.part
		Part	:Red_ElectricityArchInvert2.part
		Part	:Red_ElectricityArch2.part
		Part	:RedElectricityArchInvert.part
		Part	:RedElectricityArchWide.part
		Part	:RedElectricityArchInvertWide.part
		Part	:ElectricityArchWideWhite.part
		Part	:ElectricityArchInvertWideWhite.part
		Part	:ElectricityArchWideWhite2.part
		Part	:ElectricityArchInvertWideWhite2.part
		LifeSpan 20
	End
End

#############################################################

End