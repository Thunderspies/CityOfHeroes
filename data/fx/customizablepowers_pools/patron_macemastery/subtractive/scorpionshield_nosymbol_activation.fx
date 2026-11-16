#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\LegMotionGlows2.part
			Sound regen5b 60 60 0.5
		End

		Event
			At Hips
			Type Local
			Flags PowerLoopingSound
			PMagnet UlegL
			Bhvr behaviors/soundFade3.bhvr
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\LegMotionGlows2.part
			Sound glow5_loop 60 60 0.12
			LifeSpan 430
		End

		Event
			At Hips
			Type Local
			PMagnet UlegR
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\LegMotionGlows2.part
		End

		Event
			At LLegL
			Type Local
			PMagnet ULEGL
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\LegMotionGlows2.part
		End

		Event
			At LLegR
			Type Local
			PMagnet ULEGR
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\LegMotionGlows2.part
		End

		Event
			At LLegL
			Type Local
			PMagnet FootL
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\LegMotionGlows2.part
		End

		Event
			At LLegR
			Type Local
			PMagnet FootR
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\LegMotionGlows2.part
		End

		Event
			At FootR
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\LegMotionGlows2.part
		End

		Event
			At FootL
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\LegMotionGlows2.part
		End

		Event
			At FootR
			Type Local
			PMagnet LlegR
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ArmMotionGlows2.part
		End

		Event
			At ULegL
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ArmMotionGlows2.part
		End

		Event
			At LLegL
			Type Local
			PMagnet UlegL
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ArmMotionGlows2.part
		End

		Event
			At FootL
			Type Local
			PMagnet LlegL
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ArmMotionGlows2.part
		End

		Event
			At ULegR
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ArmMotionGlows2.part
		End

		Event
			At LLegR
			Type Local
			PMagnet UlegL
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ArmMotionGlows2.part
		End

		Event
			At Head
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MainMotionGlows2.part
		End

		Event
			At Chest
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MainMotionGlows2.part
		End

		Event
			At Neck
			Type Local
			PMagnet UarmL
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MainMotionGlows2.part
		End

		Event
			At Neck
			Type Local
			PMagnet UarmR
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MainMotionGlows2.part
		End

		Event
			At Neck
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\LegMotionGlows2.part
		End

		Event
			At UArmL
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ArmMotionGlows2.part
		End

		Event
			At LArmL
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ArmMotionGlows2.part
		End

		Event
			At HandL
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ArmMotionGlows2.part
		End

		Event
			At UArmR
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ArmMotionGlows2.part
		End

		Event
			At LArmR
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ArmMotionGlows2.part
		End

		Event
			At HandR
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ArmMotionGlows2.part
		End

	End

	Condition
		On Cycle
		Time 20
		Chance 0.2

		Event
			At LArmL
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRaysSmall2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRing2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRing2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeDots2.part
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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRaysSmall2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRing2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRing2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeDots2.part
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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRaysSmall2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRing2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRing2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeDots2.part
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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRaysSmall2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRing2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRing2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeDots2.part
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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRaysSmall2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRing2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRing2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeDots2.part
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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRaysSmall2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRing2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeRing2.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\FortitudeDots2.part
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
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\Glow01.part
		End

	End

	Condition
		On Time
		Time 28

		Event
			At Hips
			Type Local
			EName Ring
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ERing02.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\Cloud02.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\Shield01.part
		End

#		Event
#			At Chest
#			Type Local
#			EName Logo
#			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\Logo01.part
#		End
#
#		Event
#			At Waist
#			Type Local
#			EName Logo
#			BhvrOverride
#				PositionOffset 0 0.5 0
#			End
#			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\Logo02.part
#		End

	End

	Condition
		On Death

		Event
			At Hips
			Type Local
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\ERing01.part
			Part CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\Cloud01.part
			LifeSpan 1
		End

	End

End
