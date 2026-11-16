#
FxInfo

	LifeSpan 200


###########################################################

Condition
	On 	Time
	Time	18
	Event
		Type	Local
		At	Root
		Sound 	DD_ReapersBolt_Attack_01 200 100 .8
	End
End


	Condition
		On Time
		Time 18

		Event
			At WepL
			Type Start
			Part :FrigidLance_Hand_Glow.part
			Part :FrigidLance_Hand_Glow_Additive.part
			Part :FrigidLance_Hand_Snow.part
			Part :FrigidLance_Hand_Snow_Additive.part
			Part :FrigidLance_Hand_Core.part
			LifeSpan 37
		End

	End

	Condition
		On Time
		Time 20

		Event
			At WepL
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr :ReapersBolt_Projectile.bhvr
			Geom FX_ReapersBolt
		End

		Event
			AltPiv 2
			At Prime
			Type Local
			EName Pivot2
		End

		Event
			AltPiv 1
			At Prime
			Type Local
			EName Trail
			POther Pivot2
			Part :FrigidBolt_Glow.part
			Part :FrigidBolt_Core.part
			Part :FrigidBolt_Glow_Additive.part
		End

		Event
			AltPiv 3
			At Prime
			Type Local
			EName Trail
			POther Pivot2
			Part :FrigidBolt_Trail_Swirls.part
		End

		Event
			AltPiv 4
			At Prime
			Type Local
			EName Trail
			POther Pivot2
			Part :FrigidBolt_Trail_Swirls.part
		End

		Event
			At Prime
			Type Local
			EName Trail
			Part :FrigidBolt_Trail_Core.part
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
