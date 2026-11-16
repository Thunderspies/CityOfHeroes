#
FxInfo

	Input
		InpName Hips
	End


###########################################################

	Condition
		On Time
		Time 2

		Event
			At Chest
			Type Local
			Part :Flash.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Part :Glow.part
			Part :GoldenCenter.part
			#Part :Lingering_Cloud.part
			Part :Toxic_Cloud_Out.part
			Part :Lingering_Cloud_Out.part
			Part :Toxic_Splash_Continuing.part
		End

	  	Event
	  		Type	Local
	  		At	Root
	  		Bhvr	behaviors/GenericParticleFade.bhvr
	  		BhvrOverride
	  			PositionOffset	0.0 4 0.0
	  		End
			Part :Toxic_Spores_Continuing2.part
	  	End

	End

End
