#########################################################
## Electric Melee - Assassin's Strike
########################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ElectricalArc
		Type	local
		At	WepL
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:AssassinsStrike_ElectricArc1.part
		Part	:AssassinsStrike_ElectricArc2.part
		Pmagnet	WepR
		Lifespan 30
	End
End

############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Thingy
		Type	local
		At	HandR
		BHVR	POWERS\ElectricityControl\LightningHandRotateUP.bhvr
		Part	:WitchSparks.part
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part1	:ElectricityControl01White2.part
		Part2	:ElectricityControl02White2.part
		LifeSpan 20
	End

	Event
		EName 	Thingy2
		Type	local
		At	HandL
		BHVR	POWERS\ElectricityControl\LightningHandRotateLeftUP.bhvr
		Part	:WitchSparks.part
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part1	:ElectricityControl01White2.part
		Part2	:ElectricityControl02White2.part
		LifeSpan 20
	End

	Event
		EName 	Thingy3
		Type	local
		At	HandR
		BHVR	POWERS\ElectricityControl\LightningHandRotateUP.bhvr
		Part	:ElectricityOuterGlow.part
		Part	:ElectricityArch.part
		Part	:ElectricityRing.part
		Part	:ElectricityArchInvert.part
		Part	:ElectricityArch.part
		Part	:ElectricityArchInvert2.part
		Part	:ElectricityArch2.part
		Part	:ElectricityArchInvert.part
		Part	:ElectricityArchWide.part
		Part	:ElectricityArchInvertWide.part
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
		BHVR	POWERS\ElectricityControl\LightningHandRotateLeftUP.bhvr
		Part	:ElectricityOuterGlow.part
		Part	:ElectricityArch.part
		Part	:ElectricityRing.part
		Part	:ElectricityArchInvert.part
		Part	:ElectricityArch.part
		Part	:ElectricityArchInvert2.part
		Part	:ElectricityArch2.part
		Part	:ElectricityArchInvert.part
		Part	:ElectricityArchWide.part
		Part	:ElectricityArchInvertWide.part
		Part	:ElectricityArchWideWhite.part
		Part	:ElectricityArchInvertWideWhite.part
		Part	:ElectricityArchWideWhite2.part
		Part	:ElectricityArchInvertWideWhite2.part
		LifeSpan 20
	End
End

#############################################################

End