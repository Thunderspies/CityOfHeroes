#
FxInfo

	LifeSpan 100


###########################################################
Condition
	On 	Time
	Time	20
	Event
		Type	Local
		At	Root
		Sound 	SM_SkySplitter_Attack_01 100 100 .8
	End
End

	Condition
		Time 15

		Event
			EName	BeamBody
			At 	Origin
			Type	Local
			CHILDFX	:CHILD_SkySplitter_Beam_Body.fx
			Lifespan 20
		End


		Event
			EName	BeamMind
			At 	Origin
			Type	Local
			CHILDFX	:CHILD_SkySplitter_Beam_Mind.fx
			Lifespan 20
		End

		Event
			EName	BeamSoul
			At 	Origin
			Type	Local
			CHILDFX	:CHILD_SkySplitter_Beam_Soul.fx
			Lifespan 20
		End
	End

	Condition
		Time 25


		Event
			EName	StaffBody
			At 	Origin
			Type	Local
			CHILDFX	:CHILD_SkySplitter_Star_Body.fx
			Lifespan 5
		End

		Event
			EName	StaffMind
			At 	Origin
			Type	Local
			CHILDFX	:CHILD_SkySplitter_Star_Mind.fx
			Lifespan 5
		End


		Event
			EName	StaffSoul
			At 	Origin
			Type	Local
			CHILDFX	:CHILD_SkySplitter_Star_Soul.fx
			Lifespan 5
		End

	End

	Condition
		On Time
		Time 32

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 0 3
				PyrRotate 90 0 90
			End
			Part :Air_Dust_Traveling.part
			LifeSpan 3
		End

	End

	Condition
		On Time
		Time 58

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 1 7
				PyrRotate 20 15 90
				Spin 0 0.5 0
			End
			Part CustomizeablePowers\StaffFighting\Center_StaffImage_FrontFacing_Large.part
			Part CustomizeablePowers\StaffFighting\Center_Swish_FrontFacing_Large.part
			Part CustomizeablePowers\StaffFighting\Ends_Swish_Bright_FrongFacing_Large.part
			Part CustomizeablePowers\StaffFighting\Ends_Swish_FrontFacing_Large.part
			Part CustomizeablePowers\StaffFighting\Center_Swish_FrontFacing_Huge.part
			LifeSpan 2
		End

		Event
			EName	SwishBody
			At Origin
			Type Local
			CHILDFX :CHILD_SkySplitter_Swish_Body.fx
			LifeSpan 3
		End

		Event
			EName	SwishMind
			At Origin
			Type Local
			CHILDFX :CHILD_SkySplitter_Swish_Mind.fx
			LifeSpan 3
		End
		Event
			EName	SwishSoul
			At Origin
			Type Local
			CHILDFX :CHILD_SkySplitter_Swish_Mind.fx
			LifeSpan 3
		End

	End

	Condition
		On Time
		Time 60

		Event
			At Root
			Type Local
			EName StaffAnchor
			BhvrOverride
				PositionOffset 0 -1 12
			End
			LifeSpan 20
		End

		Event
			At StaffAnchor
			Type StartPositOnly
			Bhvr behaviors\CameraShake01_Strong.bhvr
			BhvrOverride
				PositionOffset 0.0 0.5 0.0
			End
			Part CustomizeablePowers\StaffFighting\DustPillar_Left.part
			Part CustomizeablePowers\StaffFighting\DustPillar_Right.part
			Part CustomizeablePowers\StaffFighting\DustPillar_Left_Lingering.part
			Part CustomizeablePowers\StaffFighting\DustPillar_Right_Lingering.part
			Part CustomizeablePowers\StaffFighting\Impact_Flash.part
			LifeSpan 5
		End

	End

End
