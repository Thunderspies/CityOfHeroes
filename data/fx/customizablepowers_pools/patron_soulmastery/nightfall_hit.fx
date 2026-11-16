#                                                          
FxInfo

	Input
		InpName Hips
	End

	LifeSpan 10


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Local
			EName Tendrills
			Part CustomizeablePowers\DarkBlast\DarkBlastHitTendril2.part
			Part CustomizeablePowers\DarkBlast\DarkBlastHitPuddle2.part
			Sound DirtExplowidow2 80 80 0.8
			LifeSpan 7
		End

	End

	Condition
		On Time
		Time 1

		Event
			At Waist
			Type Start
			EName Explode
			Part :soulFlash1.part
			Part :soulExplode2.part
			Part :soulSparks2.part
			Part :soulCloud2.part
			LifeSpan 1
		End

	End

End
