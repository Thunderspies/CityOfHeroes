#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			EName Anchor
			BhvrOverride
				PositionOffset 0 0.2 0
			End
		End

		Event
			At Anchor
			Type PositOnly
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Heartbeat_Glow.part
			Part :Heartbeat_Flash.part
			Part :Heartbeat_Ring.part
			Part IncarnatePowers\Hybrid\Heartbeat_RingFlash_Flat.part
		#	Part IncarnatePowers\Hybrid\Heartbeat_Ring_Flat.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 8

		Event
			At Anchor
			Type PositOnly
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Heartbeat_Flash_Small.part
			LifeSpan 10
		End

	End

End
