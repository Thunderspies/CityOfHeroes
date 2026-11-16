###########################################################

FxInfo

	LifeSpan 60

###########################################################

	Condition
		On Time
		Time 14

		Event
			At Root
			Type Start
			EName Hand1
			PMagnet LarmR
			Part :EngulfingDarkness_Hole_Initial.part
				Sound blackstar1 100 100 0.8
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 16

		Event
			At Root
			Type Start
			EName Hand1
			PMagnet LarmR
			Part :EngulfingDarkness_Hole.part
				Sound blackstar1 100 100 0.8
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 19

		Event
			At Root
			Type Start
			EName Hand1
			PMagnet LarmR
			Part :EngulfingDarkness_Shockwave_Initial.part
			Part :EngulfingDarkness_Shockwave_Initial_ScreenFacing.part
			Part :EngulfingDarkness_Tendrils_Exploding.part
			Sound blackstar1 100 100 0.8
			LifeSpan 10
		End

	End

	Condition
		On 	Time
		Time 	21

		Event
			At Root
			Type Start
			EName Hand1
			PMagnet LarmR
			Part :EngulfingDarkness_Hole_Steady.part
			Sound blackstar1 100 100 0.8
			LifeSpan 10
		End
	End


	Condition
		On 	Time
		Time	23
		Event
			At Root
			Type Start
			EName Hand1
			PMagnet LarmR
			Bhvr :EngulfingDarkness_Offset.bhvr
			Part :EngulfingDarkness_Shockwave.part
			Part :EngulfingDarkness_Tendrils_ScreenFacing.part
			Part :EngulfingDarkness_Tendrils_Subtractive.part
			Part :EngulfingDarkness_Tendrils_Shrinking.part
			Sound blackstar1 100 100 0.8
			LifeSpan 10
		End

	End

	Condition
		Time 40

		Event
			At Root
			Type Start
			EName Hand1
			PMagnet LarmR
			Part :EngulfingDarkness_Hole_Shrinking.part
			Sound blackstar1 100 100 0.8
			LifeSpan 40
		End

	End

###########################################################

End
