#                                                          
FxInfo

	Input
		InpName HandR
	End


###########################################################

	Condition
		On Time
		Time 20

		Event
			At HandR
			Type Local
			EName Thingy
			Sound elecwindup_loop 80 80 0.5
		End

		Event
			At HandR
			Type Local
			EName Thingy
			BhvrOverride
				FadeOutLength 200
			End
			Part :Red_ElectricityControl01.part
			Part :Red_ElectricityControl02.part
			Part :Red_ElectricityInnerGlow.part
			Part :Red_ElectricityControl01white.part
			Part :Red_ElectricityControl02white.part
		End

		Event
			At HandR
			Type Local
			EName ThingyA
			BhvrOverride
				FadeOutLength 200
			End
			Part :Red_ElectricityControl01.part
			Part :Red_ElectricityControl02.part
			Part :Red_ElectricityInnerGlow.part
			Part :Red_ElectricityControl01white.part
			Part :Red_ElectricityControl02white.part
		End

	End

End
