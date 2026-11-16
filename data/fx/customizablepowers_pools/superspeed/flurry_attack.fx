###########################################################

FxInfo

###########################################################

	Condition
		On Time
		Time 0

		Event
			At WepR
			Type Local
			EName Right
			Part :speedattack1.part
			Sound flurrypower 50 50 0.53
		End

		Event
			At WepL
			Type Local
			EName Left
			Part :speedattack1.part
		End

		Event
			At WepR
			Type Posit
			EName Right2
			Part :SpeedAttackStreaks.part
			Part :speedattack2.part
		End

		Event
			At WepL
			Type Posit
			EName Left2
			Part :SpeedAttackStreaks.part
			Part :speedattack2.part
		End

		Event
			At Chest
			Type Local
			EName aura
			PMagnet Chest
			Part :FlurryAura.part
			Part :FlurryAuraOrange.part
		End

	End

	Condition
		On Time
		Time 3

		Event
			At Chest
			Type Start
			EName BubblingSpots
			Part :BubblingSpots.part
		End

	End

	Condition
		On Time
		Time 1

		Event
			Anim Tintable_FlurryFists
			At Root
			Type Local
		#	Bhvr behaviors/speed.bhvr
			BhvrOverride
				TintGeom 1
			End
		End

	End

	Condition
		On Time
		Time 3

		Event
			Anim Tintable_FlurryFists
			At Root
			Type Local
		#	Bhvr behaviors/speed.bhvr
			BhvrOverride
				TintGeom 1
			End
		End

	End

	Condition
		On Time
		Time 6

		Event
			Anim Tintable_FlurryFists
			At Root
			Type Local
		#	Bhvr behaviors/speed.bhvr
			BhvrOverride
				TintGeom 1
			End
			Sound Flurry 50 50 0.75
		End

	End

	Condition
		On Time
		Time 9

		Event
			Anim Tintable_FlurryFists
			At Root
			Type Local
		#	Bhvr behaviors/speed.bhvr
			BhvrOverride
				TintGeom 1
			End
		End

	End

	Condition
		On Time
		Time 83

		Event
			At WepR
			Type Posit
			EName FistStar
			Part :SpeedAttackstar.part
			Sound whoosh13 80 80 0.82
		End

	End

	Condition
		On Time
		Time 83

		Event
			Type Destroy
			EName BubblingSpots
		End
	End

	Condition
		On Time
		Time 90

		Event
			Type Destroy
			EName All
		End
	End

###########################################################

End
