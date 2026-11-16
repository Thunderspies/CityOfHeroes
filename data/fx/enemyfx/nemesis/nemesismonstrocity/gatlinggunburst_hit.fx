#                                                          
FxInfo

	Input
		InpName Chest
	End

	Input
		InpName Origin
	End

	LifeSpan 25


###########################################################

	Condition
		On Time
		Time 4

		Event
			At Origin
			Type Local
			Sound machinegunhit2 60 60 0.45
		End

	End

	Condition
		On Cycle
		Time 2
		Chance 1

		Event
			At Chest
			Type Local
			EName Prime
			Part ENEMYFX\Nemesis\NemesisMonstrocity\GunHit_flash.part
			Part ENEMYFX\Nemesis\NemesisMonstrocity\GunHit_flashStar.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 0
		Repeat 1
		RepeatJitter 1

		Event
			At Chest
			Type Start
			ChildFx V_COV\PhysicsEnabled\GunSparkOUT.fx
			EName Sparkup
			BhvrOverride
				PositionOffset 0 0 0
			End
		End

	End

	Condition
		On Time
		Time 0
		Repeat 1
		RepeatJitter 1

		Event
			At Chest
			Type Start
			ChildFx V_COV\PhysicsEnabled\GunSparkOUT.fx
			EName Sparkup
			HardwareOnly 
			BhvrOverride
				PositionOffset 0 0 0
			End
		End

	End

End
