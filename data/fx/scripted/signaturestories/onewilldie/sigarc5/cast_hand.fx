#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Sound FW_Carnival_Casting_HealKneel_Loop 100 100 0.1
		End

		Event
			At WepL
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Beacon_Border_Small.part
			Part :Beacon_Core_Small.part
			Part :Beacon_Core_Small_Flash.part
			Part :Beacon_LightRays.part
			Part :Beacon_Electricity.part
			Part :Beacon_Electricity_Core.part
		End

		Event
			At WepR
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Beacon_Border_Small.part
			Part :Beacon_Core_Small.part
			Part :Beacon_Core_Small_Flash.part
			Part :Beacon_LightRays.part
			Part :Beacon_Electricity.part
			Part :Beacon_Electricity_Core.part

		End

	End

End
