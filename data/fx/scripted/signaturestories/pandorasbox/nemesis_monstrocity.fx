#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At head
			Type local
			EName Anchor
			BhvrOverride
				PositionOffset 0 -0.2 -0.3
			End
		End

		Event
			Ename PandoraPowerAnchor
			At Anchor
			Type CREATE
  			Inherit	Rotation Position
  			Update	Rotation Position
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Monstrocity_Central_Flashes01.part
			Part :Monstrocity_Central_Flashes02.part
			Part :Monstrocity_Central_Glow01.part
			Part :Monstrocity_Central_Glow02.part
			Part :Monstrocity_Central_Arcs.part
			Part :Monstrocity_BlackMatter_Inflow.part
		End
	End

End
