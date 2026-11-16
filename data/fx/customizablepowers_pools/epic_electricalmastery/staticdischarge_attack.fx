#                                                          
FxInfo

	LifeSpan 60


###########################################################

	Condition
		On Time
		Time 30

		Event
			At HandR
			Type Local
			EName Thingy
			Bhvr :WitchLightningHandRotate.bhvr
			Part :WitchSparks.part
			Part :ElectricityControl01.part
			Part :ElectricityControl02.part
			Part :WitchElectricityControl01.part
			Part :WitchElectricityControl02.part
			Part :ElectricityInnerGlow.part
			Sound StaticDischarge1 90 90 0.6
			#Sound eleccontrol2 50 40 0.4
		End

		Event
			At HandL
			Type Local
			EName Thingy2
			Bhvr :WitchLightningHandRotateLeft.bhvr
			Part :WitchSparks.part
			Part :ElectricityControl01.part
			Part :ElectricityControl02.part
			Part :WitchElectricityControl01.part
			Part :WitchElectricityControl02.part
			Part :ElectricityInnerGlow.part
		End

		Event
			At HandR
			Type Local
			EName Thingy3
			Bhvr :WitchLightningHandRotate.bhvr
			Part :ElectricityOuterGlow.part
			Part :ElectricityArch.part
			Part :ElectricityRing.part
			Part :ElectricityArchInvert.part
			Part :WitchElectricityArch.part
			Part :WitchElectricityArchInvert.part
			Part :WitchElectricityArchWide.part
			Part :WitchElectricityArchInvertWide.part
			Part :WitchElectricityArchWideWhite.part
			Part :WitchElectricityArchInvertWideWhite.part
		End

		Event
			At HandL
			Type Local
			EName Thingy4
			Bhvr :WitchLightningHandRotateLeft.bhvr
			Part :ElectricityOuterGlow.part
			Part :ElectricityArch.part
			Part :ElectricityRing.part
			Part :ElectricityArchInvert.part
			Part :WitchElectricityArch.part
			Part :WitchElectricityArchInvert.part
			Part :WitchElectricityArchWide.part
			Part :WitchElectricityArchInvertWide.part
			Part :WitchElectricityArchWideWhite.part
			Part :WitchElectricityArchInvertWideWhite.part
		End

	End

End
