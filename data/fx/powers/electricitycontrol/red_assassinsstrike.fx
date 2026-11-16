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

#############################################################

Condition
	On 	Time
	Time 	10

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