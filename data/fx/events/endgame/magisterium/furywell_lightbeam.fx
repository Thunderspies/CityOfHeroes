#
FxInfo


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Magisterium_GodBeam_loop 100 100 .3
	End
End

	Condition

		Event
			At Root
			Type Start
			EName BeginAnchor
			BhvrOverride
				PositionOffset -18 150 0
			End
		End

		Event
			At Root
			Type Start
			EName EndAnchor
			BhvrOverride
				PositionOffset 3 -25 0
			End
		End

		Event
			At Root
			Type Start
			Part Events\EndGame\Magisterium\LightDust.part
			Part Events\EndGame\Magisterium\LightDust2.part
			Part Events\EndGame\Magisterium\LightDust3.part
		End

		Event
			At BeginAnchor
			Type Local
			POther EndAnchor
			Part Events\EndGame\Magisterium\WellBeam_Add.part
			Part Events\EndGame\Magisterium\WellBeam.part
			#Part Emotes\CostumeChanges\Rainbow\Rainbow.part
			#LifeSpan 30
		End

		Event
			At Root
			Type Start
			Bhvr behaviors\Splat_RuneCircle.bhvr
			BhvrOverride
				PositionOffset 0 1 0
				Scale 30 30 30
				StartColor 255 250 230
				Stretch 0
				SplatFalloffType NONE
				SplatFlags ADDITIVE
			End
			Splat PlayerShadowCircle.tga
		End

	End

End
