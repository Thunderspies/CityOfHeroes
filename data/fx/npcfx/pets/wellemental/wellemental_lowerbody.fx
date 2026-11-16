###########################################################

FxInfo

###########################################################

	Condition
		On	Time
		Time	0

		Event
			At Chest
			Type Local
			Bhvr	behaviors\fadein.bhvr
			Flags	AdoptParentEntity
			ChildFX	:Wellemental_Spawn.fx
		End
	End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Part :Splash_Chest.part
			Part :Rays_Head.part
		End

		Event
			At Hips
			Type Local
			Part :Waterfall_Idle.part
			Part :Waterfall_Base.part
			Part :Waterfall_Movement.part

		End

		Event
			At LArmR
			Type Local
			EName ArmFX
			BhvrOverride
				PositionOffset 0.2 0.0 0.0
				PyrRotate 0 30 90
			End
			Part :Arm_Glow.part
		End

		Event
			At LarmL
			Type Local
			EName FX
			BhvrOverride
				PositionOffset -0.2 0.0 0.0
				PyrRotate 0 -30 90
			End
			Part :Arm_Glow.part
		End

		Event
			At Neck
			Type Local
			EName ArmFX
			Part :Arm_Glow_Large.part
		End



	End

	Condition
		On		TRIGGERBITS
		TRIGGERBITS 	MALE
		DoMany		1

		Event
			EName	MaleWaterfall
			At Root
			Type Local
			UNTIL	AIR
			Flags	OneAtATime
			CHILDFX	:CHILDFX_WaterfallCape_Splash.fx
		End

	End

	Condition
		On	DEATH

		Event
			EName	SpawnOut
			At	Chest
			Type	Local
			Flags	AdoptParentEntity
			Bhvr	behaviors\fadeOut.bhvr
			CHILDFX :Wellemental_Spawn.fx
		End
	End

###########################################################

End
