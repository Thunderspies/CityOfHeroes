#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Local
			Part :LegMotionGlows2.part
			Sound regen5b 60 60 0.5
		End

		Event
			At Hips
			Type Local
			Flags PowerLoopingSound
			PMagnet UlegL
			Bhvr behaviors/soundFade3.bhvr
			Part :LegMotionGlows2.part
			Sound glow5_loop 60 60 0.12
			LifeSpan 430
		End

		Event
			At Hips
			Type Local
			PMagnet UlegR
			Part :LegMotionGlows2.part
		End

		Event
			At LLegL
			Type Local
			PMagnet ULEGL
			Part :LegMotionGlows2.part
		End

		Event
			At LLegR
			Type Local
			PMagnet ULEGR
			Part :LegMotionGlows2.part
		End

		Event
			At LLegL
			Type Local
			PMagnet FootL
			Part :LegMotionGlows2.part
		End

		Event
			At LLegR
			Type Local
			PMagnet FootR
			Part :LegMotionGlows2.part
		End

		Event
			At FootR
			Type Local
			Part :LegMotionGlows2.part
		End

		Event
			At FootL
			Type Local
			Part :LegMotionGlows2.part
		End

		Event
			At FootR
			Type Local
			PMagnet LlegR
			Part :ArmMotionGlows2.part
		End

		Event
			At ULegL
			Type Local
			Part :ArmMotionGlows2.part
		End

		Event
			At LLegL
			Type Local
			PMagnet UlegL
			Part :ArmMotionGlows2.part
		End

		Event
			At FootL
			Type Local
			PMagnet LlegL
			Part :ArmMotionGlows2.part
		End

		Event
			At ULegR
			Type Local
			Part :ArmMotionGlows2.part
		End

		Event
			At LLegR
			Type Local
			PMagnet UlegL
			Part :ArmMotionGlows2.part
		End

		Event
			At Head
			Type Local
			Part :MainMotionGlows2.part
		End

		Event
			At Chest
			Type Local
			Part :MainMotionGlows2.part
		End

		Event
			At Neck
			Type Local
			PMagnet UarmL
			Part :MainMotionGlows2.part
		End

		Event
			At Neck
			Type Local
			PMagnet UarmR
			Part :MainMotionGlows2.part
		End

		Event
			At Neck
			Type Local
			Part :LegMotionGlows2.part
		End

		Event
			At UArmL
			Type Local
			Part :ArmMotionGlows2.part
		End

		Event
			At LArmL
			Type Local
			Part :ArmMotionGlows2.part
		End

		Event
			At HandL
			Type Local
			Part :ArmMotionGlows2.part
		End

		Event
			At UArmR
			Type Local
			Part :ArmMotionGlows2.part
		End

		Event
			At LArmR
			Type Local
			Part :ArmMotionGlows2.part
		End

		Event
			At HandR
			Type Local
			Part :ArmMotionGlows2.part
		End

	End

	Condition
		On Cycle
		Time 20
		Chance 0.2

		Event
			At LArmL
			Type Local
			Part :FortitudeRaysSmall2.part
			Part :FortitudeRing2.part
			Part :FortitudeRing2.part
			Part :FortitudeDots2.part
			LifeSpan 25
		End

	End

	Condition
		On Cycle
		Time 19
		Chance 0.2

		Event
			At LArmR
			Type Local
			Part :FortitudeRaysSmall2.part
			Part :FortitudeRing2.part
			Part :FortitudeRing2.part
			Part :FortitudeDots2.part
			LifeSpan 25
		End

	End

	Condition
		On Cycle
		Time 21
		Chance 0.2

		Event
			At UArmL
			Type Local
			Part :FortitudeRaysSmall2.part
			Part :FortitudeRing2.part
			Part :FortitudeRing2.part
			Part :FortitudeDots2.part
			LifeSpan 25
		End

	End

	Condition
		On Cycle
		Time 22
		Chance 0.2

		Event
			At UArmR
			Type Local
			Part :FortitudeRaysSmall2.part
			Part :FortitudeRing2.part
			Part :FortitudeRing2.part
			Part :FortitudeDots2.part
			LifeSpan 25
		End

	End

	Condition
		On Cycle
		Time 17
		Chance 0.2

		Event
			At LLegL
			Type Local
			Part :FortitudeRaysSmall2.part
			Part :FortitudeRing2.part
			Part :FortitudeRing2.part
			Part :FortitudeDots2.part
			LifeSpan 25
		End

	End

	Condition
		On Cycle
		Time 18
		Chance 0.2

		Event
			At LLegR
			Type Local
			Part :FortitudeRaysSmall2.part
			Part :FortitudeRing2.part
			Part :FortitudeRing2.part
			Part :FortitudeDots2.part
			LifeSpan 25
		End

	End

	Condition
		On Time
		Time 15

		Event
			At Hips
			Type Local
			EName Ring
			Part :Glow01.part
		End

	End

	Condition
		On Time
		Time 28

		Event
			At Hips
			Type Local
			EName Ring
			Part :ERing02.part
			Part :Cloud02.part
			Part :Shield01.part
		End

		Event
			At Chest
			Type Local
			EName Logo
			Part :Logo01.part
		End

		Event
			At Waist
			Type Local
			EName Logo
			BhvrOverride
				PositionOffset 0 0.5 0
			End
			Part :Logo02.part
		End

	End

	Condition
		On Death

		Event
			At Hips
			Type Local
			Part :ERing01.part
			Part :Cloud01.part
			LifeSpan 1
		End

	End

End
