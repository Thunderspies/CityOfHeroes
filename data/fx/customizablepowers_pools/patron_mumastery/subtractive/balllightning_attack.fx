#
FxInfo

	LifeSpan 200


###########################################################

	Condition
		On Time
		Time 15

		Event
			At WepL
			Type Start
			EName Prime
			LookAt T_chest
			Magnet T_chest
			Bhvr behaviors/MorterLobbprojectile.bhvr
			Part CustomizablePowers_Pools\Patron_MuMastery\Subtractive\Red_ElectricityArchA.part
			Part CustomizablePowers_Pools\Patron_MuMastery\Subtractive\Red_ElectricityArchInvertA.part
			Part CustomizablePowers_Pools\Patron_MuMastery\Subtractive\Red_ElectricityBallTail2A.part
			Part CustomizablePowers_Pools\Patron_MuMastery\Subtractive\Red_ElectricityBallTail2AWhite.part
			Part CustomizablePowers_Pools\Patron_MuMastery\Subtractive\Red_ElectricityBallTailA.part
			Part CustomizablePowers_Pools\Patron_MuMastery\Subtractive\Red_ElectricityBallTailWhite.part
			Part CustomizablePowers_Pools\Patron_MuMastery\Subtractive\Red_ElectricityControl01a.part
			Part CustomizablePowers_Pools\Patron_MuMastery\Subtractive\Red_ElectricityControl02a.part
			Part CustomizablePowers_Pools\Patron_MuMastery\Subtractive\Red_ElectricityRingA.part
			Part CustomizablePowers_Pools\Patron_MuMastery\Subtractive\Red_ElectricityStar.part
			Sound elecball2 76 76 0.66
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

	End

End
