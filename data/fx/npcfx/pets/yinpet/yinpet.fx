#
FxInfo

	Flags DontSuppress


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Sound MoM_Orb_Loop 100 100 0.5
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Local
			EName CrushingHateGeo
			Bhvr ENEMYFX\Praetorian\MoM\ForceBubble_Disintegration.bhvr
			BhvrOverride
				PositionOffset 0 2 0
				PyrRotateJitter 180 180 0
				Spin 0.05 0.05 0
				Alpha 255
				FadeInLength 20
				FadeOutLength 20
				StartColor 255 50 255
			End
			Geom Tintable_Dark_EarthGraspHit
		End

		Event
			At CrushingHateGeo
			Type Local
			ChildFx ENEMYFX\Praetorian\MoM\VoidOrb_Flash_Child.fx
			EName CrushingHateSwirls
			Flags OneAtATime
			Bhvr ENEMYFX\Praetorian\MoM\ForceBubble_Disintegration.bhvr
			BhvrOverride
				PyrRotateJitter -180 -180 0
				Spin 0.050 0.05 0
				Alpha 255
				FadeInLength 20
				FadeOutLength 20
				StartColor 255 255 255
			End
			Part :YinOrb.part
			Part :YinOrb_Trail.part
			Part :YinOrb_Trail_Add.part
			Part :YinOrb_Swirls.part
			Part :YinOrb_Swirls_Additive.part
			Part :YinOrb_Swirls_02.part
			Part :YinOrb_Border.part
			Geom Tintable_EarthGraspHit
		End

	End

	Condition
		On Death

		Event
			Type Destroy
			EName CrushingHateGeo
		End

		Event
			Type Destroy
			EName CrushingHateSwirls
		End

	End

End
