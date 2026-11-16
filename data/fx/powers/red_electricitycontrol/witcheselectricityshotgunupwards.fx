#########################################################
##	EBlast
########################################################
FxInfo

LifeSpan	50

################################


Condition
	On 	Time
	Time 	30

	Event
		EName 	Thingy
		Type	local
		At	HandR
		
		BHVR	:WitchLightningHandRotateUP.bhvr

		Part	:WitchSparks.part
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part1	:WitchElectricityControl01.part
		Part2	:WitchElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Sound eleccontrol2 50 50 .4
		

	End

	Event
		EName 	Thingy2
		Type	local
		At	HandL

		BHVR	:WitchLightningHandRotateLeftUP.bhvr

		Part	:WitchSparks.part
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part1	:WitchElectricityControl01.part
		Part2	:WitchElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		
	End


	Event
		EName 	Thingy3
		Type	local
		At	HandR

		BHVR	:WitchLightningHandRotateUP.bhvr

		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:ElectricityArchInvert.part
		Part4	:WitchElectricityArch.part
		Part4	:WitchElectricityArchInvert.part
		Part4	:WitchElectricityArchWide.part
		Part4	:WitchElectricityArchInvertWide.part
		Part4	:WitchElectricityArchWideWhite.part
		Part4	:WitchElectricityArchInvertWideWhite.part
	End

	Event
		EName 	Thingy4
		Type	local
		At	HandL

		BHVR	:WitchLightningHandRotateLeftUP.bhvr

		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:ElectricityArchInvert.part
		Part4	:WitchElectricityArch.part
		Part4	:WitchElectricityArchInvert.part
		Part4	:WitchElectricityArchWide.part
		Part4	:WitchElectricityArchInvertWide.part
		Part4	:WitchElectricityArchWideWhite.part
		Part4	:WitchElectricityArchInvertWideWhite.part
		
	End

End


End
