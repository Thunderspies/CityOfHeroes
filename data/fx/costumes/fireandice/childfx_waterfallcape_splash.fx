###########################################################

FxInfo

	Input
		InpName root
	End


###########################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	MALE
	DoMany		1

		Event
			EName	SplashOffset
			At 	Hips
			Type	Local
			Flags	OneAtATime
			UNTIL	BACKWARD
			BhvrOverride
				PYRRotate	270 0 270
				Scale		4.0 4.0 4.0
			End
			GEOM	FX_ConeOffsets
		End

		Event
			Ename	SplashOffset_Idle
			At 	SplashOffset
			Type	Local
			Flags	OneAtATime
			UNTIL	BACKWARD
			BhvrOverride
				PositionOffset	-0.3 0.0 -0.05
				Scale		0.2 0.2 0.2
			End
			AltPiv	1
		End

		Event
			EName	MaleWaterfall_Still
			At 	SplashOffset_Idle
			Type 	PositOnly
			Flags	OneAtATime
			UNTIL	BACKWARD
			Part :Splash_Idle.part
			Part :Splash_Idle_Additive.part
			Part :Splash_Movement.part
			Part :Splash_Idle_Flat.part
			Part :Splash_Movement_Flat.part
			Part :Splash_Spray.part
		End

	End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FEMALE
	DoMany		1

		Event
			EName	FemaleSplashOffset
			At 	Hips
			Type	Local
			Flags	OneAtATime
			UNTIL	BACKWARD
			BhvrOverride
				PYRRotate	270 0 270
				Scale		4.0 4.0 4.0
			End
			GEOM	FX_ConeOffsets
		End

		Event
			Ename	FemaleSplashOffset_Idle
			At 	FemaleSplashOffset
			Type	Local
			Flags	OneAtATime
			UNTIL	BACKWARD
			BhvrOverride
				PositionOffset	-0.3 0.0 -0.1
				Scale		0.2 0.2 0.2
			End
			AltPiv	1
		End

		Event
			EName	FemaleWaterfall_Still
			At 	FemaleSplashOffset_Idle
			Type 	PositOnly
			Flags	OneAtATime
			UNTIL	BACKWARD
			Part :Splash_Idle.part
			Part :Splash_Idle_Additive.part
			Part :Splash_Movement.part
			Part :Splash_Idle_Flat.part
			Part :Splash_Movement_Flat.part
			Part :Splash_Spray.part
		End

	End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	HUGE
	DoMany		1

		Event
			EName	HugeSplashOffset
			At 	Hips
			Type	Local
			Flags	OneAtATime
			UNTIL	BACKWARD
			BhvrOverride
				PYRRotate	270 0 270
				Scale		4.0 4.0 4.0
			End
			GEOM	FX_ConeOffsets
		End

		Event
			Ename	HugeSplashOffset_Idle
			At 	HugeSplashOffset
			Type	Local
			Flags	OneAtATime
			UNTIL	BACKWARD
			BhvrOverride
				PositionOffset	-0.35 0.0 -0.065
				Scale		0.2 0.2 0.2
			End
			AltPiv	1
		End

		Event
			EName	HugeWaterfall_Still
			At 	HugeSplashOffset_Idle
			Type 	PositOnly
			Flags	OneAtATime
			UNTIL	BACKWARD
			Part :Splash_Idle.part
			Part :Splash_Idle_Additive.part
			Part :Splash_Movement.part
			Part :Splash_Idle_Flat.part
			Part :Splash_Movement_Flat.part
			Part :Splash_Spray.part
		End

	End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	BACKWARD
	DoMany		1

		Event
			EName	SplashOffset_Back
			At 	Hips
			Type	Local
			Flags	OneAtATime
			WHILE	BACKWARD
			BhvrOverride
				PYRRotate	270 0 270
				Scale		4.0 4.0 4.0
			End
			GEOM	FX_ConeOffsets
		End

		Event
			Ename	SplashOffset_Idle_Back
			At 	SplashOffset_Back
			Type	Local
			Flags	OneAtATime
			WHILE	BACKWARD
			BhvrOverride
				PositionOffset	-1.0 0.0 -0.3
				Scale		0.2 0.2 0.2
			End
			AltPiv	1
		End

		Event
			EName	MaleWaterfall_Still_Back
			At 	SplashOffset_Idle_Back
			Type 	PositOnly
			Flags	OneAtATime
			WHILE	BACKWARD
			Part :Splash_Idle.part
			Part :Splash_Idle_Additive.part
			Part :Splash_Movement.part
			Part :Splash_Idle_Flat.part
			Part :Splash_Movement_Flat.part
			Part :Splash_Spray.part
		End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	COMBAT
	DoMany		1

		Event
			EName	SplashOffset_Back
			At 	Hips
			Type	Local
			Flags	OneAtATime
			WHILE	COMBAT
			BhvrOverride
				PYRRotate	270 0 270
				Scale		4.0 4.0 4.0
			End
			GEOM	FX_ConeOffsets
		End

		Event
			Ename	SplashOffset_Idle_Back
			At 	SplashOffset_Back
			Type	Local
			Flags	OneAtATime
			WHILE	COMBAT
			BhvrOverride
				PositionOffset	-0.2 0.0 -0.2
				Scale		0.2 0.2 0.2
			End
			AltPiv	1
		End

		Event
			EName	MaleWaterfall_Still_Back
			At 	SplashOffset_Idle_Back
			Type 	PositOnly
			Flags	OneAtATime
			WHILE	COMBAT
			Part :Splash_Movement.part
			Part :Splash_Idle_Flat.part
			Part :Splash_Movement_Flat.part
		End
	End
###########################################################

End
