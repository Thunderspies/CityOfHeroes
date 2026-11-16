#
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 40


###########################################################

	Condition
		On Time
		Time 10

		Event
			At HandR
			Type Local
			EName Thingy
			Part :Red_ElectricityControl01.part
			Part :Red_ElectricityControl02.part
			Part :Red_ElectricityInnerGlow.part
			Sound elecattack2 50 50 0.8
		End

		Event
			At HandL
			Type Local
			EName Thingy2
			Part :Red_ElectricityControl01.part
			Part :Red_ElectricityControl02.part
			Part :Red_ElectricityInnerGlow.part
		End

		Event
			At HandR
			Type Local
			EName Thingy3
			Part :Red_ElectricityOuterGlow.part
			Part :Red_ElectricityArch.part
			Part :Red_ElectricityRing.part
			Part :Red_ElectricityArchInvert.part
		End

		Event
			At HandL
			Type Local
			EName Thingy4
			Part :Red_ElectricityOuterGlow.part
			Part :Red_ElectricityArch.part
			Part :Red_ElectricityRing.part
			Part :Red_ElectricityArchInvert.part
		End

	End

End
