#                                                          
FxInfo


###########################################################

	Condition
		On Cycle
		Time 20
		Chance 0.2

		Event
			At LArmL
			Type Local
			EName 3
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
			EName 3
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
			EName 3
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
			EName 3
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
			EName 3
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
			EName 3
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
			EName Ring
			Part :ERing01.part
			Part :Cloud01.part
			LifeSpan 1
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Local
			EName 3
			Part :LegMotionGlows2.part
			Sound regen5b 60 60 0.5
		End

		Event
			At Hips
			Type Local
			EName 3
			PMagnet UlegL
			Part :LegMotionGlows2.part
			Sound glow5_loop 60 60 0.12
		End

		Event
			At Hips
			Type Local
			EName 3
			PMagnet UlegR
			Part :LegMotionGlows2.part
		End

		Event
			At LLegL
			Type Local
			EName 3
			PMagnet ULEGL
			Part :LegMotionGlows2.part
		End

		Event
			At LLegR
			Type Local
			EName 6
			PMagnet ULEGR
			Part :LegMotionGlows2.part
		End

		Event
			At LLegL
			Type Local
			EName 4
			PMagnet FootL
			Part :LegMotionGlows2.part
		End

		Event
			At LLegR
			Type Local
			EName 7
			PMagnet FootR
			Part :LegMotionGlows2.part
		End

		Event
			At FootR
			Type Local
			EName 2
			Part :LegMotionGlows2.part
		End

		Event
			At FootL
			Type Local
			EName 5
			Part :LegMotionGlows2.part
		End

		Event
			At FootR
			Type Local
			EName 2
			PMagnet LlegR
			Part :ArmMotionGlows2.part
		End

		Event
			At ULegL
			Type Local
			EName 3
			Part :ArmMotionGlows2.part
		End

		Event
			At LLegL
			Type Local
			EName 4
			PMagnet UlegL
			Part :ArmMotionGlows2.part
		End

		Event
			At FootL
			Type Local
			EName 5
			PMagnet LlegL
			Part :ArmMotionGlows2.part
		End

		Event
			At ULegR
			Type Local
			EName 6
			Part :ArmMotionGlows2.part
		End

		Event
			At LLegR
			Type Local
			EName 7
			PMagnet UlegL
			Part :ArmMotionGlows2.part
		End

		Event
			At Head
			Type Local
			EName 9
			Part :MainMotionGlows2.part
		End

		Event
			At Chest
			Type Local
			EName 16
			Part :MainMotionGlows2.part
		End

		Event
			At Neck
			Type Local
			EName 17
			PMagnet UarmL
			Part :MainMotionGlows2.part
		End

		Event
			At Neck
			Type Local
			EName 18
			PMagnet UarmR
			Part :MainMotionGlows2.part
		End

		Event
			At Neck
			Type Local
			EName 19
			Part :LegMotionGlows2.part
		End

		Event
			At UArmL
			Type Local
			EName 10
			#PMagnet ElbowL
			Part :ArmMotionGlows2.part
		End

		Event
			At LArmL
			Type Local
			EName 11
			#PMagnet HandL
			Part :ArmMotionGlows2.part
		End

		Event
			At HandL
			Type Local
			EName 12
			Part :ArmMotionGlows2.part
		End

		Event
			At UArmR
			Type Local
			EName 13
			#PMagnet ElbowL
			Part :ArmMotionGlows2.part
		End

		Event
			At LArmR
			Type Local
			EName 14
			#PMagnet HandL
			Part :ArmMotionGlows2.part
		End

		Event
			At HandR
			Type Local
			EName 15
			Part :ArmMotionGlows2.part
		End

	End

End
