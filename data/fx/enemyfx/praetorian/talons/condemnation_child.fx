#                                                          
FxInfo

	LifeSpan 100


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			EName RingsOut
			Part :CoreRingsOut.part
			Part :RingsOut.part
			Part :CoreRings.part
			Part :RingsOut2.part
			Part ENEMYFX\Praetorian\Talons\DarkTendrilCloud_B.part
			LifeSpan 15
		End

	End

	Condition
		On Time
		Time 2

		Event
			At Origin
			Type Start
			Part ENEMYFX\Praetorian\Talons\Darkl_Ring.part
			#Part CustomizeablePowers\RadiationBlast\Radiationflash.part
		End

	End

End
