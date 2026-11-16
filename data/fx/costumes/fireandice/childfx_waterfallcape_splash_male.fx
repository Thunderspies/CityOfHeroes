###########################################################

FxInfo

	Input
		InpName Root
	End


###########################################################

Condition
	On			TRIGGERBITS
	TRIGGERBITS		MALE
	DoMany		1

		Event
			EName	SplashOffsetA
			At 	Hips
			Type	Local
			Flags	OneAtATime
			UNTIL	COMBAT
			BhvrOverride
				PYRRotate	270 0 270
				Scale		4.0 4.0 4.0
			End
			GEOM	FX_ConeOffsets
		End

####################### IDLE #############################

		Event
			Ename	SplashOffset
			At 	SplashOffsetA
			Type	Local
			Flags	OneAtATime
			UNTIL	COMBAT
			BhvrOverride
				PositionOffset	0.0 -0.0 -0.0
			End
			AltPiv	2
		End

####################### MOVEMENT #########################

		Event
			EName	MaleWaterfall_Movement
			At 	SplashOffset
			Type 	PositOnly
			Flags	OneAtATime
			UNTIL	COMBAT
			Part :Splash_Idle.part
			Part :Splash_Idle_Additive.part
			Part :Splash_Idle_Flat.part
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
			Part :Splash_Idle.part
			Part :Splash_Idle_Additive.part
			Part :Splash_Idle_Flat.part
			Part :Splash_Spray.part
		End
End


###########################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FORWARD
	DoMany		1

		Event
			Ename	FWSplashOffset
			At 	SplashOffsetA
			Type	Local
			WHILE	FORWARD
			Flags	OneAtATime
			AltPiv	2
		End

		Event
			EName	MaleWaterfall_FOREWARD
			At 	FWSplashOffset
			Type 	PositOnly
			WHILE	FORWARD
			Flags	OneAtATime
			Part :Splash_Movement_Flat.part
			Part :Splash_Movement.part
			Part :Splash_Idle_Flat.part
		End
End

###########################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	BACKWARD
	DoMany		1

		Event
			Ename	BWSplashOffset
			At 	SplashOffsetA
			Type	Local
			WHILE	BACKWARD
			Flags	OneAtATime
			BhvrOverride
				PositionOffset	-0.0 0.15 -0.2
			End
			AltPiv	2
		End

		Event
			EName	MaleWaterfall_Backward
			At 	BWSplashOffset
			Type 	PositOnly
			WHILE	BACKWARD
			Flags	OneAtATime
			Part :Splash_Movement_Flat.part
			Part :Splash_Movement.part
			Part :Splash_Idle_Flat.part
		End
End

###########################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	STEPLEFT
	DoMany		1

		Event
			Ename	SLSplashOffset
			At 	SplashOffsetA
			Type	Local
			WHILE	STEPLEFT
			Flags	OneAtATime
			AltPiv	2
		End

		Event
			EName	MaleWaterfall_STEPLEFT
			At 	SLSplashOffset
			Type 	PositOnly
			WHILE	STEPLEFT
			Flags	OneAtATime
			Part :Splash_Movement_Flat.part
			Part :Splash_Movement.part
			Part :Splash_Idle_Flat.part
		End
End
###########################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	STEPRIGHT
	DoMany		1

		Event
			Ename	SRSplashOffset
			At 	SplashOffsetA
			Type	Local
			WHILE	STEPRIGHT
			Flags	OneAtATime
			AltPiv	2
		End

		Event
			EName	MaleWaterfall_STEPRIGHT
			At 	SRSplashOffset
			Type 	PositOnly
			WHILE	STEPRIGHT
			Flags	OneAtATime
			Part :Splash_Movement_Flat.part
			Part :Splash_Movement.part
			Part :Splash_Idle_Flat.part
		End
End

###########################################################

End
