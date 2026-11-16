#                                                          
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 200


###########################################################

	Condition
		On Time
		Time 19

		Event
			At Root
			Type Local
			EName roothookup
			Bhvr behaviors/alpha.bhvr
			Geom RootToChestAdjustment
		End

		Event
			AltPiv 1
			At roothookup
			Type Local
			EName hookup
			Geom LungeNode
		End

		Event
			AltPiv 2
			At hookup
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr :Shuriken.bhvr
			Part :ShurikenStreaks01.part
			Geom Tintable_GEO_WepR_Shuriken
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

	End

End
