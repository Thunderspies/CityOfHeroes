#
FxInfo

	LifeSpan 60


###########################################################
Condition
	On 	Time
	Time	20
	Event
		Type	Local
		At	Root
		Sound 	TOV_Banshee_Lamentation 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
	#		Sound ScreamPBAOE 100 100 1.0
		End

		Event
			At Head
			Type Local
			EName LocknarDummyHookup
		End

		Event
			At Root
			Type Local
			EName HookUp
			Geom RootToHeadAdjustment
			LifeSpan 40
		End

		Event
			At Head
			Type Local
			EName Rings
			Part :AOE_CoreRings.part
			Part :AOE_RingsOut2.part
			LifeSpan 40
		End

		Event
			AltPiv 1
			At HookUp
			Type Local
			EName RingsDown
			BhvrOverride
				PositionOffset 0 1.7 -0.3
			End
			Part :CoreRingsUp.part
			Part :CoreRingsUp2.part
			Part :RingsUp.part
			Part :RingsUp2.part
			Part :MussleFlashRing2.part
			Part :MussleFlashRingCore2.part
			Part :DarkBreath_Big.part
			LifeSpan 40
		End

		Event
			At Root
			Type Local
			EName RingsOut
			Part :AOE_CoreRingsOut.part
			Part :AOE_RingsOut.part
			Part :AOE_DarkGround.part
			LifeSpan 40
		End

	End

	Condition
		On Cycle
		Time 16

		Event
			At Head
			Type Start
			EName VibrationForce
			BhvrOverride
				physForcePower 75
				physForcePowerJitter 25
				physForceRadius 55
				physForceType Up
			End
			LifeSpan 1
		End

		Event
			At Head
			Type Start
			EName ExplosiveForce
			BhvrOverride
				physForceType Out
				physForceRadius 60
				physForcePower 25
				physForcePowerJitter 10
			End
			LifeSpan 1
		End

	End

	Condition
		On Time
		Time 16

		Event
			At Head
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\ShadowField_Expand_Large.bhvr
			BhvrOverride
				Scale 3 3 3
				EndScale 4 4 4
				Alpha 60
				PositionOffset 0 0 0
				#TintGeom 1
				StartColor 1 1 1
				Spin 0 -0.2 0
				SpinJitter 0.0001 0.0001 0.0001
			End
			#Geom FX_Singularity_Ring
			Geom Tintable_ShadowField_Highlights
			LifeSpan 10
		End

		Event
			At Root
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\ShadowField_Expand_Large.bhvr
			BhvrOverride
				Scale 3 3 3
				EndScale 12 12 12
				Alpha 75
				PositionOffset 0 3 0
				StartColor 1 1 1
				Spin 0 -0.2 0
				SpinJitter 0.0001 0.0001 0.0001
			End
			Geom FX_Singularity_Ring
			LifeSpan 10
		End

		Event
			At Root
			Type Local
			Bhvr CustomizeablePowers\DarknessControl\ShadowField_Expand_Large.bhvr
			BhvrOverride
				Scale 3 3 3
				EndScale 12 12 12
				Alpha 120
				PositionOffset 0 3 0
				PyrRotate 0 180 0
				StartColor 1 1 1
				Spin 0 -0.2 0
				SpinJitter 0.0001 0.0001 0.0001
			End
			Geom FX_Singularity_Ring
			LifeSpan 10
		End

	End

End
