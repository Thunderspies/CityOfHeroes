#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName BoxTopStartAnchor
			BhvrOverride
				PositionOffset 0 2.25 6
			End
		End

		Event
			At Root
			Type Local
			EName BoxBottomAnchor
			BhvrOverride
				PositionOffset 0 0 6
			End
		End

		Event
			At BoxBottomAnchor
			Type Local
			Geom _SS_PB_Pandora_Box_Bottom
		End

		Event
			At BoxTopStartAnchor
			Type Local
			Geom _SS_PB_Pandora_Box_Top
		End

		Event
			At BoxBottomAnchor
			Type Local
			BhvrOverride
				PositionOffset 0 2.25 0
				Scale 1 0.8 1
				StartColor 255 220 50
			End
			Part Scripted\SignatureStories\PandorasBox\Glow_ClosedBox_Add.part
			Part Scripted\SignatureStories\PandorasBox\Glow_ClosedBox.part
			Geom PandoraBox_FX_Streak_A
		End

		Event
			At BoxBottomAnchor
			Type Local
			BhvrOverride
				PositionOffset 0 2.25 0
				StartColor 255 200 0
			End
			Geom PandoraBox_FX_Streak_C
		End

	End

	Condition
		On Time
		Time 0

		Event
			At BoxTopStartAnchor
			Type Local
			Bhvr behaviors\fadein.bhvr
			Geom _SS_PB_Pandora_Box_decal_top
		End

		Event
			At BoxBottomAnchor
			Type Local
			Bhvr behaviors\fadein.bhvr
			Geom _SS_PB_Pandora_Box_decal_bottom
		End

	End

End
