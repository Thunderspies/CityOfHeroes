###########################################################

FxInfo

	Input
		InpName root
	End


###########################################################

Condition
	On	Time
		Event
			EName	SplashOffset
			At 	Hips
			Type	Local
			BhvrOverride
				PYRRotate	270 0 270
				Scale		4.0 4.0 4.0
			End
			GEOM	FX_ConeOffsets
		End

###########################################################

		Event
			Ename	SplashOffset_Left
			At 	SplashOffset
			Type	Local
			BhvrOverride
				PositionOffset	-0.0 0.1 -0.05
				Scale		0.1 0.1 0.1
			End
			AltPiv	2
		End

		Event
			Ename	SplashOffset_Right
			At 	SplashOffset
			Type	Local
			BhvrOverride
				PositionOffset	-0.0 -0.1 -0.05
				Scale		0.1 0.1 0.1
			End
			AltPiv	2
		End



###########################################################

		Event
			EName	MaleWaterfall_Movement_L
			At 	SplashOffset_Left
			Type 	PositOnly
			Part :Splash_Movement_Double.part
			Part :Splash_Idle_Flat_Double.part
			Part :Splash_Movement_Flat.part
			Part :Splash_Spray_Double.part
		End

		Event
			EName	MaleWaterfall_Movement_R
			At 	SplashOffset_Right
			Type 	PositOnly
			Part :Splash_Movement_Double.part
			Part :Splash_Idle_Flat_Double.part
			Part :Splash_Movement_Flat_Double.part
			Part :Splash_Spray_Double.part
		End
End

###########################################################

End
