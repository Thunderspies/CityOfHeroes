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
			Part ENEMYFX\Praetorian\Talons\DarkCloud.part
			Part ENEMYFX\Praetorian\Talons\DarkSmoke_Trail02.part
			Part ENEMYFX\Praetorian\Talons\DarkTendrilCloud.part
			Part ENEMYFX\Praetorian\Talons\DarkTendrils.part
			Part ENEMYFX\Praetorian\Talons\DarkTendrilCloud_B.part
			Part ENEMYFX\Praetorian\Talons\DarkBlobs_Big.part
			Part ENEMYFX\Praetorian\Talons\DarkTendrils_Big.part
			Part ENEMYFX\Praetorian\Talons\DarkGlow.part
			Part ENEMYFX\Praetorian\Talons\SonicRing_Damage01.part
			Part ENEMYFX\Praetorian\Talons\SonicRing_Damage02.part
			Part ENEMYFX\Praetorian\Talons\SonicRing_Damage03.part
			Part ENEMYFX\Praetorian\Talons\SonicRing_Damage04.part
			Part :AOE_CoreRingsOut.part
			Part :AOE_RingsOut.part
			Part :AOE_DarkGround.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 2

		Event
			At Origin
			Type PositOnly
			Part ENEMYFX\Praetorian\Talons\Dark_Ring.part
			Part :AOE_CoreRingsOut.part
			Part :AOE_RingsOut.part
			LifeSpan 15
		End

	End

End
