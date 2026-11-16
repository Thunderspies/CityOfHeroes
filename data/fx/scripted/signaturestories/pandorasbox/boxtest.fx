#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName BoxAnchor
			BhvrOverride
				PositionOffset 0 0 6
			End
		End

		Event
			At BoxAnchor
			Type Local
			Geom _SS_PB_Pandora_Box
		End

		Event
			At BoxAnchor
			Type Local
			Geom _SS_PB_Pandora_Box_decal_deactivated
		End

		Event
			At BoxAnchor
			Type Local
			BhvrOverride
				PositionOffset 0 2.25 0
				Scale 1 0.5 1
				StartColor 255 220 50
			End
			Part Scripted\SignatureStories\PandorasBox\Glow_ClosedBox_Add.part
			Part Scripted\SignatureStories\PandorasBox\Glow_ClosedBox.part
			Geom PandoraBox_FX_Streak_A
		End

		Event
			At BoxAnchor
			Type Local
			BhvrOverride
				PositionOffset 0 2.22 0
				Alpha 150
				StartColor 255 240 100
			End
			#Geom PandoraBox_FX_Streak_B
		End

		Event
			At BoxAnchor
			Type Local
			BhvrOverride
				PositionOffset 0 2.25 0
				StartColor 255 200 0
			End
			Geom PandoraBox_FX_Streak_C
		End

	End

End
