#
FxInfo

	LifeSpan 75

	ClampMinScale 0.875 0.875 0.875
	ClampMaxScale 1.125 1.125 1.125

###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Part :DarkHit.part
			Part :DarkHit1.part
			Part :DarkHit2.part
			Part :DarkHitRaysSmall.part
			Part :DarkSmoke_Hit01.part
		End

		Event
			At Chest
			Type Local
			Part :DarkHitStar01Sml.part
			Part :DarkHitStar02Sml.part
			Part :DarkHitStar03Sml.part
			Part :DarkHitGlow01Sml.part
			Part :DarkHitRays01Sml.part
			Sound pp13 100.0 100.0 0.7
		End

		Event
			EName	ArrowHitHookup
			Type	create
			Inherit	Position
			Update	Position
			At	CHEST
			LookAt	Target
		End

		Event
			EName	ArrowHit
			Type	create
			Inherit	Position rotation
			Update	Position
			At	ArrowHitHookup
		End

		Event
			Ename	Arrow
			Type	Local
			At	ArrowHit
			Bhvr	WEAPONS\Bow\Archery\ArrowsHitFade.bhvr
			BhvrOverride
				PyrRotate 0 90 0
				PositionOffset -2.4 0 0
			End
			Geom	ObsidianArrow
			Lifespan	44
		End

	End

End
