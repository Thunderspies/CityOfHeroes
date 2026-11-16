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
			Geom geo_fx_testsphere
		End

		Event
			At Anchor
			Type Start
			EName Bottom
			BhvrOverride
				PositionOffset 0 48 0
			End
			Geom geo_fx_testsphere
		End

		Event
			At Anchor
			Type Start
			EName Middle
			BhvrOverride
				PositionOffset 0 26 0
			End
			Part :Central_Flashes01.part
			Part :Central_Flashes02.part
			Part :Central_Glow01.part
			Part :Central_Glow02.part
			#Part Scripted\SignatureStories\PandorasBox\PandoraAura\Matter_Big.part
			#Part Scripted\SignatureStories\PandorasBox\PandoraAura\Matter_Clump.part
			Part Scripted\SignatureStories\PandorasBox\Central_Arcs.part
			Part Scripted\SignatureStories\PandorasBox\BlackMatter_Inflow.part
			Geom geo_fx_testsphere
		End

		Event
			At Middle
			Type Local
			BhvrOverride
				PyrRotate 180 0 0
			End
			Part Scripted\SignatureStories\PandorasBox\BlackMatter_Inflow.part
		End

		Event
			At Anchor
			Type Start
			EName Top
			BhvrOverride
				PositionOffset 0 48 0
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
			Part :Electric_CylinderGlow.part
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
