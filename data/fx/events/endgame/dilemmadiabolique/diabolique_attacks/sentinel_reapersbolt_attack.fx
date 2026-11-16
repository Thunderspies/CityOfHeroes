#
FxInfo

	LifeSpan 200


###########################################################


Condition
	On 	Time
	Time	37
	Event
		Type	Local
		At	Root
		Sound 	DD_SentinelReapersBolt_Attack_01 300 100 .8
	End
End


	Condition
		On Time
		Time 37

		Event
			At Head
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr :ReapersBolt_Projectile.bhvr
			BhvrOverride
				PositionOffset 0 -2 0
				Scale 5 5 5
				StartColor 0 30 50
			End
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
			Part :FrigidBolt_Glow_Sentinel.part
			Part :FrigidBolt_Core_Sentinel.part
			Part :FrigidBolt_Glow_Additive_Sentinel.part
		End

		Event
			AltPiv 3
			At Prime
			Type Local
			EName Trail
			POther Pivot2
			Part :FrigidBolt_Trail_Swirls_Sentinel.part
		End

		Event
			AltPiv 4
			At Prime
			Type Local
			EName Trail
			POther Pivot2
			Part :FrigidBolt_Trail_Swirls_Sentinel.part
		End

		Event
			At Prime
			Type Local
			EName Trail
			Part :FrigidBolt_Trail_Core_Sentinel.part
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
