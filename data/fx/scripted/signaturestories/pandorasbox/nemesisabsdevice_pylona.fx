#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName Anchor
			BhvrOverride
				#PositionOffset 0 9 6
			End
			Part Scripted\SignatureStories\PandorasBox\Pylon_Flash01.part
			Part Scripted\SignatureStories\PandorasBox\Pylon_Flash02.part
			Part Scripted\SignatureStories\PandorasBox\Pylon_Glow01.part
			Part Scripted\SignatureStories\PandorasBox\Pylon_Glow02.part
			Part Scripted\SignatureStories\PandorasBox\BlackMatter_DotsUp.part
			Part Scripted\SignatureStories\PandorasBox\BlackMatter_Sm.part
		End

		Event
			At Anchor
			Type Start
			EName Bottom
			BhvrOverride
				PositionOffset 0 38 0
			End
			Geom geo_fx_testsphere
		End

		Event
			At Bottom
			Type Local
			EName CentralArc
		End

		Event
			At bottom
			Type Local
			EName CylinderGlow
			LookAt Top
			POther Top
			Part Scripted\SignatureStories\PandorasBox\Electric_CylinderGlow2.part
			#Part :Electric_CylinderGlow.part
		End

	End

	Condition
		On Cycle
		Time 20

		Event
			At bottom
			Type Local
			EName CylinderArcsUp
			LookAt Top
			POther Top
			Part :Electric_VerticalStream.part
			Part :Electric_VerticalStream_Down.part
			LifeSpan 15
		End

	End

End
