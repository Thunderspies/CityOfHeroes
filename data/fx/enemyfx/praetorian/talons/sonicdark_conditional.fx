#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			EName RingsOut
			#Part :DarkCloud.part
			#Part :DarkSmoke_Trail02.part
			#Part :DarkTendrilCloud.part
			#Part :DarkTendrils.part
			#Part :DarkTendrilCloud_B.part
			#Part :DarkBlobs_Big.part
			#Part :DarkTendrils_Big.part
			#Part :DarkGlow.part
			#Part :SonicRing_Dot01.part
			#Part :SonicRing_Dot02.part
			Part :SonicRing_Dot03.part
			Part ENEMYFX\Praetorian\Talons\DarkGlow_Continuing.part
			Part ENEMYFX\Praetorian\Talons\DarkTendrils_Continuing.part
			Part ENEMYFX\Praetorian\Talons\DarkBlobs_Dot.part
			Part :SonicRing_Dot04.part
			#LifeSpan 10
		End

	End

	Condition
		On Time
		Time 2

		Event
			At Origin
			Type Start
			Part :Dark_Ring.part
			#Part CustomizeablePowers\RadiationBlast\Radiationflash.part
		End

	End

End
