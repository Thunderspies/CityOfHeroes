#
FxInfo

	LifeSpan 100


###########################################################

Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	ToV_Keres_EnervatingBolt_01 200 100 .8
	End
End

	Condition
		On Time
		Time 10

		#Event
			#At WepL
			#Type Local
			#EName LeftHandMist
			#Sound bittericethrow3 100.0 100.0 0.85
			#LifeSpan 37
		#End

		Event
			At WepR
			Type Local
			EName RightHandMist
			Part :Keres_Hand_Glow.part
			Part :Keres_Hand_Flash_Additive.part
			Part :Keres_Hand_DarkClouds.part
			Part :Keres_Hand_DarkBlobs.part
			Part :Keres_Hand_Core.part
			LifeSpan 37
		End

	End

	Condition
		On Time
		Time 55

		Event
			At WepR
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr ENEMYFX\Praetorian\Talons\Medianprojectile1.bhvr
			BhvrOverride
				TrackRate 3
				#Alpha 0
				StartColor 24 195 255
			End
		End

		Event
			At Prime
			Type Local
			EName Trail
			BhvrOverride
				PositionOffset 0 0 -2
				PyrRotate 80 90 90
				Scale 0.1 0.1 0.4
				Spin 0 0.1 0
				#Alpha 120
			End
			Part :Tracer_Trail.part
			Part :icebolttail3.part
			Part :icebolttail2.part
			Part :IceBoltMistTrail2.part
			Part :EnergyProjectileStreaks.part
			Part ENEMYFX\Praetorian\Talons\Comet_Sm.part
			Part ENEMYFX\Praetorian\Talons\Comet_Sm_Dark.part
			Geom Glacier_Chunk_05
		End

		Event
			At Prime
			Type Local
			EName Trail
			Part :Keres_Snow.part
			Part :Keres_DarkSmoke.part
			Part ENEMYFX\Praetorian\Talons\ColdSmoke_Trail01.part
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
