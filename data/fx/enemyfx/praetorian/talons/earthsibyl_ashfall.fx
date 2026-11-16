#
FxInfo

	Flags DontSuppress

	LifeSpan 350


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	ToV_EarthSibyl_Ashfall_01 300 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At T_Root
			Type Start
			EName TopAnchor
			BhvrOverride
				PositionOffset 0 60 0
			End
		End

		Event
			At T_Root
			Type Start
			EName BottomAnchor
		End

		Event
			At TopAnchor
			Type Local
			Part :AshCloud_Falling.part
			Part :AshCloud_Falling_Streams.part
			Part :DirtStreaks_Falling.part
			LifeSpan 270
		End

	End

	Condition
		On Time
		Time 30

		Event
			At BottomAnchor
			Type Local
			Part :AshCloud_Spread.part
			Part :AshCloud_Ground.part
			LifeSpan 270
		End

		Event
			At BottomAnchor
			Type Start
			EName PHYSICS-ExplosiveForce-
			BhvrOverride
				physForceType Out
				physForceRadius 15
				physForcePower 600
				physForcePowerJitter 75
			End
			LifeSpan 1
		End

	End

	Condition
		On Time
		Time 40

		Event
			At BottomAnchor
			Type Start
			Bhvr behaviors\CameraShake_Subtle90.bhvr
			Part :AshCloud_Impact.part
			LifeSpan 10
		End

		Event
			At BottomAnchor
			Type Local
			EName RingAnchor
			Bhvr ENEMYFX\Praetorian\Talons\Ground_Dust_Expand.bhvr
			BhvrOverride
				PositionOffset 0 3.5 0
			End
			Part ENEMYFX\Praetorian\Talons\Dust_Ring_Expanding.part
			LifeSpan 0
		End

	End

	Condition
		On Time
		Time 300

		Event
			At BottomAnchor
			Type Local
			Part :AshCloud_DustSettle.part
			LifeSpan 5
		End

	End

End
