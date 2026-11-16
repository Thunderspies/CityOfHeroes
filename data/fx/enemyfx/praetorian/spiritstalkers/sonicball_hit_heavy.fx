#                                                          
FxInfo

	LifeSpan 100


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type PositOnly
			EName RingsOut
			Part ENEMYFX\Praetorian\Talons\SonicRing_Damage01.part
			Part ENEMYFX\Praetorian\Talons\SonicRing_Damage02.part
			Part ENEMYFX\Praetorian\Talons\SonicRing_Damage03.part
			Part ENEMYFX\Praetorian\Talons\SonicRing_Damage04.part
			Part ENEMYFX\Praetorian\SpiritStalkers\AOE_CoreRingsOut.part
			Part ENEMYFX\Praetorian\SpiritStalkers\AOE_RingsOut.part
			Part ENEMYFX\Praetorian\SpiritStalkers\AOE_GroundSwirl.part
			Part ENEMYFX\Praetorian\SpiritStalkers\EnergyBlobs_Big.part
			Part ENEMYFX\Praetorian\SpiritStalkers\Tendrils_Big.part
			Part ENEMYFX\Praetorian\SpiritStalkers\BigHit_Glow.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 2

		Event
			At Origin
			Type PositOnly
			#Part ENEMYFX\Praetorian\Talons\Dark_Ring.part
			Part ENEMYFX\Praetorian\Talons\AOE_CoreRingsOut.part
			Part ENEMYFX\Praetorian\Talons\AOE_RingsOut.part
			LifeSpan 15
		End

	End

End
