#
FxInfo

	LifeSpan 300


###########################################################

Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	ToV_Keres_DeadlyBlast_01 200 100 .8
	End
End

	Condition
		On Time
		Time 10

		Event
			At WepR
			Type Local
			EName RightHandMist
			Part :Keres_Hand_Glow.part
			Part :Keres_Hand_Flash_Additive.part
			Part :Keres_Hand_DarkClouds.part
			Part :Keres_Hand_DarkBlobs.part
			Part :Keres_Hand_Core.part
			LifeSpan 15
		End

	End

	Condition
		On Time
		Time 25

		Event
			At WepR
			Type Local
			EName RightHandMist
			Part ENEMYFX\Praetorian\Talons\Keres_Hand_BigFlash_Additive.part
			Part ENEMYFX\Praetorian\Talons\Keres_Hand_BigDarkClouds.part
			Part ENEMYFX\Praetorian\Talons\Keres_Hand_BigDarkBlobs.part
			Part ENEMYFX\Praetorian\Talons\Keres_Hand_BigCore.part
			Part ENEMYFX\Praetorian\Talons\Keres_Hand_Glow.part
			Part ENEMYFX\Praetorian\Talons\Keres_Hand_LightBlobs.part
			LifeSpan 27
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
				#TrackRate 3
				#Alpha 0
				StartColor 24 195 255
			End
		End

		Event
			At Prime
			Type Local
			EName Trail
			BhvrOverride
				PositionOffset 0 0 1
				PyrRotate 80 90 90
				Scale 0.2 0.2 0.250
				Spin 0 0.1 0
				#Alpha 120
			End
			Part :icebolttail2.part
			Part :icebolttail3.part
			Part :EnergyProjectileStreaks.part
			Part :IceBoltMistTrail2.part
			Part :Tracer_Trail_02.part
			Part :Comet.part
			Part :Keres_DarkSmoke_Trail02.part
			Part :Keres_DarkSmoke_local.part
			Part :Comet_Dark.part
			Part :Keres_DarkBlobs2.part
			Part :ColdSmoke_Trail01.part
			Part :ColdSmoke_Trail02.part
			Part :ColdSmoke_Trail04.part
			Geom Glacier_Chunk_02
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
